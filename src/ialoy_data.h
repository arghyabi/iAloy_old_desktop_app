#ifndef IALOY_DATA_H
#define IALOY_DATA_H

#include <iostream>
#include "hardware.h"

using namespace std;

enum email_connected_pi_status
{
	INVALID_USER_REGISTERED_PI = 0,
	PERMANENT_USER_REGISTERED_PI,
	SUPER_USER_REGISTERED_PI,
	PERMANENT_USER_UNREGISTERED_PI,
	INVALID_USER_UNREGISTERED_PI

};

enum setUpLineEditStat
{
	GET_INPUT_EMAIL_MODE = 0,
	GET_INPUT_PROD_KEY_MODE,
	GET_INPUT_PASSWORD_MODE,
	GET_INPUT_OTP_MODE,
	GET_INPUT_PI_NAME_MODE
};

enum product_Key_status
{
	PRODUCT_KEY_AVAILABLE = 0,
	PRODUCT_KEY_NOT_AVAILABLE,
	PRODUCT_KEY_INVALID
};

enum pi_reg_status
{
	PI_NOT_REGISTERED = 0,
	PI_REGISTERED
};

enum reg_user_type
{
	EXISTING_USER = 0,
	NEW_USER
};

enum user_login_status
{
	LOGIN_FAIL = 0,
	LOGIN_SUCCESS,
	LOGIN_TOKEN_ERROR
};

enum otp_send_status
{
	OTP_SEND_FAIL = 0,
	OTP_SEND_SUCCESS
};

enum otp_verification_status
{
	OTP_NOT_VERIFIED = 0,
	OTP_VERIFIED
};

enum api_request_type{
	CHECK_PI_STATUS = 0,
	GET_PI_NAME,
	CHECK_EMAIL_STATUS,
	CHECK_EMAIL_CONNECTED_PI,
	GET_USER_DETAILS,
	CHECK_PRODUCT_KEY_STATUS,
	SEND_OTP,
	VERIFY_OTP,
	REGISTER_NEW_PI,
	LOGIN,
	VERIFY_TOKEN
};

class ialoy_main_data{

private:
	string pi_add, pi_name, email, product_id, password, first_name, last_name, phone,\
		 token, otp, api_error_msg, api_response;

public:
	pi_reg_status pi_reg_status_flag;
	api_request_type api_request;
	email_connected_pi_status email_connected_pi_status_flag;
	user_login_status user_login_status_flag;
	reg_user_type reg_user_type_flag;
	product_Key_status product_Key_status_flag;
	otp_send_status otp_send_status_flag;
	otp_verification_status otp_verification_status_flag;

	// setter methods for private variables.
	void set_email(string email_id);
	void set_password(string pass);
	void set_first_name(string f_name);
	void set_last_name(string l_name);
	void set_phone(string phone);
	void set_pi_add();
	void set_pi_name(string pi_name);
	void set_product_id(string prod_id);
	void set_token(string token);
	void set_otp(string otp);
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
	string get_token();
	string get_otp();
	string get_api_error_msg();
	string get_api_response();
};

#endif // IALOY_DATA_H
