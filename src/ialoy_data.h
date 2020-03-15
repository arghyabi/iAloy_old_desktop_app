#ifndef IALOY_DATA_H
#define IALOY_DATA_H

#include <iostream>
#include "hardware.h"

using namespace std;

enum api_request_type{
	CHECK_PI_STATUS = 0,
	GET_PI_NAME,
	CHECK_EMAIL_STATUS,
	CHECK_EMAIL_CONNECTED_PI,
	GET_USER_DETAILS,
	CHECK_PRODUCT_KEY_STATUS,
	SEND_OTP,
	REGISTER_NEW_PI,
	LOGIN,
	VERIFY_TOKEN
};

class ialoy_main_data{

private:
	string pi_add, pi_name, email, product_id, password, first_name, last_name, phone,\
		 otp, api_error_msg, api_response;
	int reg_user_type;

public:
	api_request_type api_request;

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
	void set_api_error_msg(string msg);
	void set_api_response(string response);

	// getter methods for private variables.
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
	string get_api_error_msg();
	string get_api_response();
};

#endif // IALOY_DATA_H
