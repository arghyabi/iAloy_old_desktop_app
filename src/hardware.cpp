 #include<iostream>
#include<fstream>

#include "hardware.h"

using namespace std;

string get_serial()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string line;

#ifdef ARC_TYPE
	system("cat /proc/cpuinfo | grep 'Serial' | awk '{print $3}' > tms.txt");
	ifstream myfile("tms.txt");
	getline(myfile, line);
	myfile.close();
	system("rm tms.txt");
#else
	line = "0123456789ABCDEF";
#endif
	return line;
}

