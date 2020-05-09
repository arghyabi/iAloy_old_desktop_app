#include <iostream>
#include <unistd.h>
#ifdef ARC_TYPE
#include <wiringPiI2C.h>
#endif
#include "i2c_data.h"

using namespace std;

i2c_data::i2c_data(QObject *parent) : QObject(parent)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

void i2c_data::write_i2c_data(int mod_address, int data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "mod address: " << mod_address << " Data: " << data << endl;
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(mod_address);
	if (fd == -1) {
		cout << "Failed to init I2C communication." << endl;
	}
	cout << "I2C communication successfully setup." << endl;

	uint8_t end_data = 0x0A;
	int data_to_send[8];

	for(int i=0; i<8; i++)
	{
		data_to_send[i] = data%2;
		data=data/2;
	}

	for(int i=0;i<8;i++)
	{
		wiringPiI2CWrite(fd, data_to_send[i]);
		for(int t = 0; t < 1000000; t++);
	}
	wiringPiI2CWrite(fd, end_data);
	close(fd);
#endif
}

void i2c_data::read_i2c_data(int mod_address, int curerent_data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(mod_address);
	for(int t = 0; t < 1000000; t++);
	int tmp_data = wiringPiI2CRead(fd);
	int received_data = 0;

	close(fd);

	for(int i = 0; i < 8; i++)
	{
		received_data = received_data << 1;
		received_data = received_data | (tmp_data & 1);
		tmp_data = tmp_data >> 1;
	}
	//received_data = received_data << 1;

	cout << "\n\n*****Mod_Add : " << mod_address <<"\tcurrent data : " << curerent_data << "\trecieved data : " << received_data << endl;
	if(curerent_data != received_data)
		emit receive_i2c_data_from_module(mod_address, received_data);
#endif
}

void i2c_data::read_all_i2c_module_state(int *data_array)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
#ifdef ARC_TYPE
	for(int i = 3; i < 120; i++)
	{
		int fd = wiringPiI2CSetup(i);
		int received_data = wiringPiI2CRead(fd);

		if(received_data != -1)
			data_array[i-3] = 1;
		else
			data_array[i-3] = 0;

		close(fd);
	}
#else
	for(int i = 3; i < 120; i++)
	{
		if(i%2 == 1)
			data_array[i-3] = 1;
		else
			data_array[i-3] = 0;
	}
#endif
}