#include <iostream>
#include <string>
#include <sstream>

#include <QtNetwork/QNetworkAccessManager>
#include "ialoy_data.h"

// saved_credential_manager
bool ialoy_main_data::saved_credential_manager()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QFile file;
	file.setFileName(QString::fromStdString(ialoy_main_data::get_user_credential_path()));

	if(!file.exists())
		return false;

	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString val = file.readAll();
	file.close();
	cout << "Raw file data : " << endl;

	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject sett2 = d.object();
	QJsonValue value = sett2.value(QString("userCredential"));
	QJsonObject item = value.toObject();

	if(item["email"] != "" && item["token"] != "")
	{
		ialoy_main_data::set_email(item["email"].toString().toStdString());
		ialoy_main_data::set_token(item["token"].toString().toStdString());

		cout << "Email : " << item["email"].toString().toStdString() << endl;
		cout << "Token : " << item["token"].toString().toStdString() << endl;

		return true;
	}
	else
	{
		cout << "No saved user credential found..." << endl;
		return false;
	}
}

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

void ialoy_main_data::set_login_manager_api_response(int response_type, string response)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	switch(response_type)
	{
		case CHECK_PI_STATUS:
			this->check_pi_status_response = response;
			break;

		case GET_PI_NAME:
			this->get_pi_name_response = response;
			break;

		case CHECK_EMAIL_STATUS:
			this->check_email_status_response = response;
			break;

		case CHECK_EMAIL_CONNECTED_PI:
			this->check_email_connected_pi_response = response;
			break;

		case GET_USER_DETAILS:
			this->get_user_details_response = response;
			break;

		case CHECK_PRODUCT_KEY_STATUS:
			this->check_product_key_status_response = response;
			break;

		case SEND_OTP:
			this->send_otp_response = response;
			break;

		case VERIFY_OTP:
			this->verify_otp_response = response;
			break;

		case REGISTER_NEW_PI:
			this->register_new_pi_response = response;
			break;

		case LOGIN:
			this->login_response = response;
			break;

		case LOGIN_USING_TOKEN:
			this->login_using_token_response = response;
			break;

	}
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

void ialoy_main_data::set_device_controller_api_response(int response_type, string resp)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	switch(response_type)
	{
		case DEVICE_CONTROLLER_LOGIN_USING_TOKEN:
			this->device_controller_login_using_token_response = resp;
			break;
		case GET_DEVICE_DETAIL_STATUS:
			this->get_device_detail_status_response = resp;
			break;
		case GET_ROOM_DEVICE_LIST:
			this->get_room_device_list_response = resp;
			break;
		case GET_ROOM_DEVICE_STATUS:
			this->get_room_device_status_response = resp;
			break;
		case UPDATE_STATUS:
			this->update_status_response = resp;
			break;
		case UPDATE_RANGE:
			this->update_range_response = resp;
			break;
		case UPDATE_STATUS_FOR_PI:
			this->update_status_for_pi_response = resp;
			break;
		case UPDATE_RANGE_FOR_PI:
			this->update_range_for_pi_response = resp;
			break;
		case GET_I2C_DATA:
			this->get_i2c_data_response = resp;
			break;
		case ADD_NEW_MODULE:
			this->add_new_module_response = resp;
			break;
		case VERIFY_PASSWORD:
			this->verify_password_response = resp;
			break;
		case UPDATE_PASSWORD:
			this->update_password_response = resp;
			break;
	}
}

void ialoy_main_data::set_device_controller_api_error_msg(string msg)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->device_controller_api_error_msg = msg;
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

string ialoy_main_data::get_login_manager_api_response(int response_type)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	switch(response_type)
	{
		case CHECK_PI_STATUS:
			return this->check_pi_status_response;
			break;
		case GET_PI_NAME:
			return this->get_pi_name_response;
			break;
		case CHECK_EMAIL_STATUS:
			return this->check_email_status_response;
			break;
		case CHECK_EMAIL_CONNECTED_PI:
			return this->check_email_connected_pi_response;
			break;
		case GET_USER_DETAILS:
			return this->get_user_details_response;
			break;
		case CHECK_PRODUCT_KEY_STATUS:
			return this->check_product_key_status_response;
			break;
		case SEND_OTP:
			return this->send_otp_response;
			break;
		case VERIFY_OTP:
			return this->verify_otp_response;
			break;
		case REGISTER_NEW_PI:
			return this->register_new_pi_response;
			break;
		case LOGIN:
			return this->login_response;
			break;
		case LOGIN_USING_TOKEN:
			return this->login_using_token_response;
			break;
		default:
			break;
	}
	return "";
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

string ialoy_main_data::get_update_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->update_icon_path;
}

string ialoy_main_data::get_power_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->power_icon_path;
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

string ialoy_main_data::get_ic_normal_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->ic_narmal_icon_path;
}

string ialoy_main_data::get_ic_plus_white_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->ic_plus_white_icon_path;
}

string ialoy_main_data::get_ic_plus_brown_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->ic_plus_brown_icon_path;
}

string ialoy_main_data::get_ic_warning_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->ic_warning_icon_path;
}

string ialoy_main_data::get_refresh_icon_path()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->refresh_icon_path;
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

string ialoy_main_data::get_device_controller_api_response(int response_type)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	switch(response_type)
	{
		case DEVICE_CONTROLLER_LOGIN_USING_TOKEN:
			return this->device_controller_login_using_token_response;
			break;
		case GET_DEVICE_DETAIL_STATUS:
			return this->get_device_detail_status_response;
			break;
		case GET_ROOM_DEVICE_LIST:
			return this->get_room_device_list_response;
			break;
		case GET_ROOM_DEVICE_STATUS:
			return this->get_room_device_status_response;
			break;
		case UPDATE_STATUS:
			return this->update_status_response;
			break;
		case UPDATE_RANGE:
			return this->update_range_response;
			break;
		case UPDATE_STATUS_FOR_PI:
			return this->update_status_for_pi_response;
			break;
		case UPDATE_RANGE_FOR_PI:
			return this->update_range_for_pi_response;
			break;
		case GET_I2C_DATA:
			return this->get_i2c_data_response;
			break;
		case ADD_NEW_MODULE:
			return this->add_new_module_response;
			break;
		case VERIFY_PASSWORD:
			return this->verify_password_response;
			break;
		case UPDATE_PASSWORD:
			return this->update_password_response;
			break;
	}
	return NULL;
}

string ialoy_main_data::get_device_controller_api_error_msg()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	return this->device_controller_api_error_msg;
}

string ialoy_main_data::int_to_hex(int int_data)
{
	string hex_data = "0x";
	stringstream sstream;
	sstream << uppercase << std::hex << int_data;
	hex_data += sstream.str();
	return hex_data;
}

int ialoy_main_data::hex_to_int(string hex_data)
{
	int int_data = 0;
	int tmp = 1;

	for(int i = 2; i < 4; i++)
	{
		switch(hex_data[i])
		{
			case '0':
				int_data = tmp * int_data;
				break;
			case '1':
				int_data = tmp * int_data + 1;
				break;
			case '2':
				int_data = tmp * int_data + 2;
				break;
			case '3':
				int_data = tmp * int_data + 3;
				break;
			case '4':
				int_data = tmp * int_data + 4;
				break;
			case '5':
				int_data = tmp * int_data + 5;
				break;
			case '6':
				int_data = tmp * int_data + 6;
				break;
			case '7':
				int_data = tmp * int_data + 7;
				break;
			case '8':
				int_data = tmp * int_data + 8;
				break;
			case '9':
				int_data = tmp * int_data + 9;
				break;
			case 'a':
			case 'A':
				int_data = tmp * int_data + 10;
				break;
			case 'b':
			case 'B':
				int_data = tmp * int_data + 11;
				break;
			case 'c':
			case 'C':
				int_data = tmp * int_data + 12;
				break;
			case 'd':
			case 'D':
				int_data = tmp * int_data + 13;
				break;
			case 'e':
			case 'E':
				int_data = tmp * int_data + 14;
				break;
			case 'f':
			case 'F':
				int_data = tmp * int_data + 15;
				break;
			default:
				break;
		}
		tmp *= 10;
	}

	return int_data;
}