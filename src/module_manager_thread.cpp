#include "module_manager_thread.h"

module_manager_thread::module_manager_thread(QObject *parent) : QObject(parent)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

int module_manager_thread::hex_to_int(string hex_data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	char *ptr;
	int int_data = strtoul(hex_data.c_str(), &ptr, 16);
	if ( *ptr != 0 )
	{
		cout << "not a number" << endl;
		return 0;
	}
	return int_data;
}

string module_manager_thread::int_to_hex(int int_data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string hex_data = "0x";
	stringstream sstream;
	sstream << uppercase << std::hex << int_data;
	hex_data += sstream.str();
	return hex_data;
}

string module_manager_thread::generate_checksum(string checksum_input)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	int checksum = 0;
	cout << "checksum i/p:" << checksum_input << endl;
	for(int i = 0; i < checksum_input.length(); i+=2)
	{
		int value = hex_to_int(checksum_input.substr(i, 2));
		checksum = (checksum + value) & 0xFF;
	}

	if(checksum)
		checksum = 256 - checksum;
	string str_checksum = int_to_hex(checksum).substr(2);
	if(str_checksum.length() == 1)
	{
		str_checksum = "0" + str_checksum;
	}

	cout << "checksum:" << str_checksum << endl;
	return str_checksum;
}

string module_manager_thread::modify(int address)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string str_address = int_to_hex(address << 1);
	string first;
	string last;
	if(str_address.substr(2,1) != "")
		first = str_address.substr(2,1);
	else
		first = "0";

	if(str_address.substr(3,1) != "")
		last = str_address.substr(3,1);
	else
		last = "0";

	string final_data = "8" + last + "E" + first + "8093";
	cout << "first:" << first << " last:" << last << " Final:" << final_data << endl;
	return final_data;
}

string module_manager_thread::modify_code(string data, int address)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string prefix = data.substr(0, 33);
	string suffix = "";
	cout << "prefix: " << prefix << endl;
	cout << "suffix: " << suffix << endl;
	string modified_data = modify(address);
	string checksum_input = prefix.substr(1) + modified_data + suffix;
	string checksum = generate_checksum(checksum_input);


	string final_string = prefix + modified_data + suffix + checksum;
	cout << "final string:" << final_string << endl;
	return final_string;
}

void module_manager_thread::burn_module_slot(int address)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string line;
	ifstream myfile("/usr/share/iAloy/hex/template.hex");
	ofstream outfile("/usr/share/iAloy/.temp/out-file.hex");
	int line_index = 1;
	if (myfile.is_open() && outfile.is_open())
	{
		while (getline(myfile,line))
		{
			if(line_index == 138)
				line = modify_code(line, address);
			outfile << line << endl;
			line_index++;
			// for(long long int i = 0; i < 10000; i++)
			// 	cout << i << endl;
		}
		myfile.close();
		outfile.close();
	}

	else
		cout << "Unable to open file";

	system("avrdude -C/usr/share/arduino/conf/avrdude.conf -v -v -v -v -patmega328p -carduino -P/dev/ttyUSB0 -b115200 -D -Uflash:w:/usr/share/iAloy/.temp/out-file.hex:i -l/usr/share/iAloy/.temp/avrdude.log");

	system("/usr/share/iAloy/.temp/out-file.hex");

	emit burn_complete_signal();
}