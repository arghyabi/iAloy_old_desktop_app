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

// setter methods for dev_controller class private variables.

void ialoy_main_data::set_d_id(string d_id)
{
	this->d_id = d_id;
}

void ialoy_main_data::set_status(string status)
{
	this->status = status;
}

void ialoy_main_data::set_range(string range)
{
	this->range = range;
}

void ialoy_main_data::set_mod_add(string mod_add)
{
	this->mod_add = mod_add;
}

void ialoy_main_data::set_pin(string pin)
{
	this->pin = pin;
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
	if(this->pi_add == "")
		set_pi_add();
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

// getter methods for path_variables

string ialoy_main_data::get_user_credential_path()
{
	return this->user_credential_path;
}

string ialoy_main_data::get_mainwindow_bg_file_path()
{
	return this->mainwindow_bg_file_path;
}

string ialoy_main_data::get_dashboard_bg_file_path()
{
	return this->dashboard_bg_file_path;
}

string ialoy_main_data::get_keyboad_icon_path()
{
	return this->keyboad_icon_path;
}

string ialoy_main_data::get_settings_icon_path()
{
	return this->settings_icon_path;
}

string ialoy_main_data::get_wifi_icon_path()
{
	return this->wifi_icon_path;
}

string ialoy_main_data::get_interface_up_script_path()
{
	return this->interface_up_script_path;
}



// getter methods for dev_controller class private variables.

string ialoy_main_data::get_d_id()
{
	return this->d_id;
}

string ialoy_main_data::get_status()
{
	return this->status;
}
string ialoy_main_data::get_range()
{
	return this->range;
}
string ialoy_main_data::get_mod_add()
{
	return this->mod_add;
}
string ialoy_main_data::get_pin()
{
	return this->pin;
}
