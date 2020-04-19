#include <iostream>
#ifdef ARC_TYPE
#include <wiringPiI2C.h>
#endif
#include "i2c.h"

using namespace std;
#define MOD_ADD 0x04

void dec_to_bin(int data, int *bin_arr)
{
	for(int i=0; i<8; i++)
	{
		bin_arr[i] = data%2;
		data=data/2;
	}
}

int random_genaration()
{
	srand(time(0));
	int n = 0;
	for (int i=0;i<10;i++)
	{
		int tmp = rand()%2;
		n = n*2 + tmp;
	}
	return n;
}

int write_data(int *data_to_send)
{
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(MOD_ADD);
	if (fd == -1) {
		cout << "Failed to init I2C communication." << endl;
		return -1;
	}
	cout << "I2C communication successfully setup." << endl;

	uint8_t end_data = 0x0A;

	for(int i=0,j=0;i<8;i++)
	{
		wiringPiI2CWrite(fd, data_to_send[i]);
		for(int t=0;t<1000000;t++);

		if(i%2==0 && j <= 7)
			j++;
	}
	wiringPiI2CWrite(fd, end_data);
#endif
	return 0;
}

int read_i2c_data(int *data_to_recv)
{
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(MOD_ADD);
	for(int t=0;t<1000000;t++);
	int received_data = wiringPiI2CRead(fd);
	dec_to_bin(received_data, data_to_recv);
#endif
	return 0;
}

int i2c_comm(int mode)
{
	int data_to_send[8];
	int data_to_recv[8];

#ifdef ARC_TYPE
	cout << endl << " this is ARM arch" << endl << endl;
#else
	cout << endl << " this is Intel arch" << endl << endl;
#endif

	if(mode == 0)
	{
		cout << "========= Read mode ==========" << endl;
		read_i2c_data(data_to_recv);

		cout << "Received_data: ";
		for(int i = 7; i > -1; i--)
		{
			cout << data_to_recv[i] << " ";
		}
		cout << endl;
	}

	else if(mode == 1)
	{
		cout << "========= Write mode ==========" << endl;
		dec_to_bin(random_genaration(), data_to_send);
		write_data(data_to_send);
		cout << "Written_data: ";
		for(int i = 0; i < 8; i++)
		{
			cout << data_to_send[i] << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "Wrong input" << endl;
	}
	return 0;
}