// 
#include<iostream>
#include<fstream>
#include "hardware.h"

using namespace std;

string get_serial()
{
	string line;

	system("cat /proc/cpuinfo | grep 'Serial' | awk '{print $3}' > tms.txt");
	ifstream myfile("tms.txt");
	getline(myfile, line);
	myfile.close();
	system("rm tms.txt");

	if(line == "")
	{
		line = "0123456789ABCDEF";
		//cout << "Intel architechture" << endl;
	}

	return line;
}

