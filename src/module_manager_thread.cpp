#include "module_manager_thread.h"

module_manager_thread::module_manager_thread(QObject *parent) : QObject(parent)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	mProcess = new QProcess(this);

	connect(mProcess, SIGNAL(started()), this, SLOT(start_burning_tmp_slot()));
	connect(mProcess, SIGNAL(readyReadStandardOutput()),this,SLOT(burning_module_tmp_slot()));
	connect(mProcess, SIGNAL(finished(int)), this, SLOT(burn_complete_tmp_slot(int)));
	connect(mProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
	{
		cout << "error enum val = " << error << endl;
	});
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
	string final_data;

	if(str_address.substr(3,1) == "")
	{
		first = "0";
		last = str_address.substr(2,1);
	}
	else
	{
		first = str_address.substr(2,1);
		last = str_address.substr(3,1);
	}

	switch(this->module_type_flag)
	{
		case ATMEGA328P:
		{
			final_data = "8" + last + "E" + first + "8093";
			break;
		}
		case ATTINY88:
		{
			final_data = "91F78" + last + "E" + first;
			break;
		}
	}


	cout << "first:" << first << " last:" << last << " Final:" << final_data << endl;
	return final_data;
}

string module_manager_thread::modify_code(string data, int address)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string prefix;
	string suffix;

	switch(this->module_type_flag)
	{
		case ATMEGA328P:
		{
			prefix = data.substr(0, 33);
			suffix = "";
			break;
		}
		case ATTINY88:
		{
			prefix = data.substr(0, 25);
			suffix = data.substr(33, 8);
			break;
		}
	}

	cout << "prefix: " << prefix << endl;
	cout << "suffix: " << suffix << endl;
	string modified_data = modify(address);
	string checksum_input = prefix.substr(1) + modified_data + suffix;
	string checksum = generate_checksum(checksum_input);


	string final_string = prefix + modified_data + suffix + checksum;
	cout << "final string:" << final_string << endl;
	return final_string;
}

void module_manager_thread::burn_module_slot(int address, int module_type_index)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << " address:" << address << endl;
	system("rm -rf /usr/share/iAloy/.temp/*.hex");

	string line;
	int line_index = 1;
	QString avrdude = "avrdude";
	QStringList arguments;
	this->module_type_flag = (module_type)module_type_index;
	ofstream outfile("/usr/share/iAloy/.temp/out-file.hex");

	switch (this->module_type_flag)
	{
		case ATTINY88:
		{
			ifstream myfile("/usr/share/iAloy/hex/template_for_attiny88.hex");
			if (myfile.is_open() && outfile.is_open())
			{
				while (getline(myfile,line))
				{
					if(line_index == 124)
						line = modify_code(line, address);
					outfile << line << endl;
					line_index++;
				}
				myfile.close();
				outfile.close();
			}
			else
				cout << "Unable to open file";

			arguments << "-v" << "-C/usr/share/arduino/conf/avrdude.conf" << "-pattiny88" << "-clinuxgpio" << "-e" << "-Uflash:w:/usr/share/iAloy/.temp/out-file.hex" << "-l/usr/share/iAloy/.temp/avrdude.log";
			break;
		}

		case ATMEGA328P:
		{
			ifstream myfile("/usr/share/iAloy/hex/template_for_atmega328.hex");
			if (myfile.is_open() && outfile.is_open())
			{
				while (getline(myfile,line))
				{
					if(line_index == 138)
						line = modify_code(line, address);
					outfile << line << endl;
					line_index++;
				}
				myfile.close();
				outfile.close();
			}
			else
				cout << "Unable to open file";

			arguments << "-v" << "-C/usr/share/arduino/conf/avrdude.conf" << "-pm328p" << "-clinuxgpio" << "-e" << "-Uflash:w:/usr/share/iAloy/.temp/out-file.hex" << "-l/usr/share/iAloy/.temp/avrdude.log";
			break;
		}
	}

	cout << "Selected Module type : " << this->module_type_flag << endl;

	mProcess->start(avrdude, arguments);
}

void module_manager_thread::start_burning_tmp_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	emit start_burning_signal();
}

void module_manager_thread::burning_module_tmp_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	emit console_print_signal(mProcess->readAllStandardOutput());
}

void module_manager_thread::burn_complete_tmp_slot(int temp)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	emit burn_complete_signal();
}
