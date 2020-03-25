#include <iostream>
#include <string>

#include "ialoy_data.h"

// define the class methods

// all setter methods
void ialoy_main_data::set_email(string email_id)
{
	this->email = email_id;
}

void ialoy_main_data::set_first_name(string f_name)
{
	this->first_name = f_name;
}

void ialoy_main_data::set_last_name(string l_name)
{
	this->last_name = l_name;
}

void ialoy_main_data::set_password(string pass)
{
	this->password = pass;
}

void ialoy_main_data::set_phone(string phone_no)
{
	this->phone = phone_no;
}

void ialoy_main_data::set_pi_add()
{
	this->pi_add = get_serial();
}

void ialoy_main_data::set_pi_name(string pi_name)
{
	this->pi_name = pi_name;
}

void ialoy_main_data::set_product_id(string prod_id)
{
	this->product_id = prod_id;
}

void ialoy_main_data::set_token(string token)
{
	this->token = token;
}

void ialoy_main_data::set_otp(string otp_data)
{
	this->otp = otp_data;
}

void ialoy_main_data::set_api_error_msg(string msg)
{
	this->api_error_msg = msg;
}

void ialoy_main_data::set_api_response(string response)
{
	this->api_response = response;
}


// all getter methods
string ialoy_main_data::get_email()
{
	return this->email;
}

string ialoy_main_data::get_first_name()
{
	return this->first_name;
}

string ialoy_main_data::get_last_name()
{
	return this->last_name;
}

string ialoy_main_data::get_password()
{
	return this->password;
}

string ialoy_main_data::get_phone()
{
	return this->phone;
}

string ialoy_main_data::get_pi_add()
{
	return this->pi_add;
}

string ialoy_main_data::get_pi_name()
{
	return this->pi_name;
}

string ialoy_main_data::get_product_id()
{
	return this->product_id;
}

string ialoy_main_data::get_token()
{
	return this->token;
}

string ialoy_main_data::get_otp()
{
	return this->otp;
}

string ialoy_main_data::get_api_error_msg()
{
	return this->api_error_msg;
}

string ialoy_main_data::get_api_response()
{
	return this->api_response;
}
