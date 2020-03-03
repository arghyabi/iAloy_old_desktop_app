#ifndef IALOY_DATA_H
#define IALOY_DATA_H

#include <iostream>
#include "hardware.h"

using namespace std;

class ialoy_main_data{

private:
	string pi_add, pi_name, email, product_id, password, first_name, last_name, phone, otp;
	int reg_user_type;

public:
	// setter methods for private variables.
	void set_email(string email_id);
	void set_password(string pass);
	void set_first_name(string f_name);
	void set_last_name(string l_name);
	void set_phone(string phone);
	void set_pi_add();
	void set_pi_name(string pi_name);
	void set_product_id(string prod_id);
	void set_otp(string otp);
	void set_user_type(int type);

	string get_email();
	string get_password();
	string get_first_name();
	string get_last_name();
	string get_phone();
	string get_pi_add();
	string get_pi_name();
	string get_product_id();
	string get_otp();
	int get_user_type();
};

#endif // IALOY_DATA_H
