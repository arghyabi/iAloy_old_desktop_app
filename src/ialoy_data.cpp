#include <iostream>
#include <string>

#include "ialoy_data.h"

// define the class methods

// all setter methods
void ialoy_main_data::set_email(string email_id)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->email = email_id;
}

void ialoy_main_data::set_first_name(string f_name)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->first_name = f_name;
}

void ialoy_main_data::set_last_name(string l_name)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->last_name = l_name;
}

void ialoy_main_data::set_password(string pass)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->password = pass;
}

void ialoy_main_data::set_phone(string phone_no)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->phone = phone_no;
}

void ialoy_main_data::set_pi_add()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->pi_add = get_serial();
}

void ialoy_main_data::set_pi_name(string pi_name)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->pi_name = pi_name;
}

void ialoy_main_data::set_product_id(string prod_id)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->product_id = prod_id;
}

void ialoy_main_data::set_token(string token)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->token = token;
}

void ialoy_main_data::set_otp(string otp_data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->otp = otp_data;
}

void ialoy_main_data::set_api_error_msg(string msg)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->api_error_msg = msg;
}

void ialoy_main_data::set_api_response(string response)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->api_response = response;
}

// setter methods for dev_controller class private variables.

void ialoy_main_data::set_d_id(string d_id)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->d_id = d_id;
}

void ialoy_main_data::set_status(string status)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->status = status;
}

void ialoy_main_data::set_range(string range)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->range = range;
}

void ialoy_main_data::set_mod_add(string mod_add)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->mod_add = mod_add;
}

void ialoy_main_data::set_pin(string pin)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->pin = pin;
}

// all getter methods
string ialoy_main_data::get_email()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->email;
}

string ialoy_main_data::get_first_name()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->first_name;
}

string ialoy_main_data::get_last_name()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->last_name;
}

string ialoy_main_data::get_password()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->password;
}

string ialoy_main_data::get_phone()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->phone;
}

string ialoy_main_data::get_pi_add()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(this->pi_add == "")
		set_pi_add();
	return this->pi_add;
}

string ialoy_main_data::get_pi_name()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->pi_name;
}

string ialoy_main_data::get_product_id()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->product_id;
}

string ialoy_main_data::get_token()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->token;
}

string ialoy_main_data::get_otp()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->otp;
}

string ialoy_main_data::get_api_error_msg()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->api_error_msg;
}

string ialoy_main_data::get_api_response()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->api_response;
}

// getter methods for path_variables

string ialoy_main_data::get_user_credential_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->user_credential_path;
}

string ialoy_main_data::get_mainwindow_bg_file_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->mainwindow_bg_file_path;
}

string ialoy_main_data::get_dashboard_bg_file_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->dashboard_bg_file_path;
}

string ialoy_main_data::get_keyboad_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->keyboad_icon_path;
}

string ialoy_main_data::get_settings_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->settings_icon_path;
}

string ialoy_main_data::get_wifi_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->wifi_icon_path;
}

string ialoy_main_data::get_interface_up_script_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->interface_up_script_path;
}



// getter methods for dev_controller class private variables.

string ialoy_main_data::get_d_id()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->d_id;
}

string ialoy_main_data::get_status()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->status;
}
string ialoy_main_data::get_range()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->range;
}
string ialoy_main_data::get_mod_add()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->mod_add;
}
string ialoy_main_data::get_pin()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->pin;
}
