#ifndef IALOY_DATA_H
#define IALOY_DATA_H

#include <iostream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QLinkedList>

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

enum api_request_type
{
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
	LOGIN_USING_TOKEN
};

enum device_controller_api_request_type
{
	DEVICE_CONTROLLER_LOGIN_USING_TOKEN = 0,
	GET_DEVICE_DETAIL_STATUS,
	GET_ROOM_DEVICE_LIST,
	GET_ROOM_DEVICE_STATUS,
	UPDATE_STATUS,
	UPDATE_RANGE,
	GET_CONNECTED_PI_LIST,
	UPDATE_STATUS_FOR_PI,
	UPDATE_RANGE_FOR_PI,
	GET_I2C_DATA
};

class ialoy_main_data{

private:
	// for setup class
	string pi_add, pi_name, product_id, password, first_name, last_name, phone,\
		 otp, api_error_msg, api_response, device_controller_api_response, device_controller_api_error_msg;

	string email, token;

	string user = "/usr/";
	string share = "share/";
	string iAloy_folder = "iAloy/";

	string conf_folder = ".conf/";
	string photos_folder = "photos/";
	string scripts_folder = "scripts/";

	string ucp_file = "credential.json";
	string mainwindow_bg_file = "mainwindow.jpg";
	string dashboard_bg_file = "dashboard.jpg";
	string keyboad_icon = "keyboard.png";
	string settings_icon = "setting.png";
	string wifi_icon = "wifi.png";
	string update_icon = "update.png";
	string power_icon = "power.png";
	string interface_up_script = "iAloy-interface-up.sh";

	string user_credential_path = user + share + iAloy_folder + conf_folder + ucp_file;
	string mainwindow_bg_file_path = user + share + iAloy_folder + photos_folder + mainwindow_bg_file;
	string dashboard_bg_file_path = user + share + iAloy_folder + photos_folder + dashboard_bg_file;
	string keyboad_icon_path = user + share + iAloy_folder + photos_folder + keyboad_icon;
	string settings_icon_path = user + share + iAloy_folder + photos_folder + settings_icon;
	string wifi_icon_path = user + share + iAloy_folder + photos_folder + wifi_icon;
	string update_icon_path = user + share + iAloy_folder + photos_folder + update_icon;
	string power_icon_path = user + share + iAloy_folder + photos_folder + power_icon;
	string interface_up_script_path = user + share + iAloy_folder + scripts_folder + interface_up_script;

	// for dev_controller class
	string d_id, status, range, mod_add, pin;

public:
	pi_reg_status pi_reg_status_flag;
	api_request_type api_request;
	email_connected_pi_status email_connected_pi_status_flag;
	user_login_status user_login_status_flag;
	reg_user_type reg_user_type_flag;
	product_Key_status product_Key_status_flag;
	otp_send_status otp_send_status_flag;
	otp_verification_status otp_verification_status_flag;
	device_controller_api_request_type device_controller_api_request_type_flag;

	string api_i2c_data;

	// global network_manager variable
	QNetworkAccessManager *NetworkManager, *DBNetworkManager;
	QNetworkRequest NetworkRequest;

	struct btn_node
	{
		QPushButton *btn;
		int btn_state;
		QSlider *slider;
		bool is_var;
		int slider_val;
		QString device_id;
		int pin_num;
		int mod_add;
	};

	struct mod_data_node
	{
		int mod_add;
		int current_web;
		int prev_web;
		int current_mod;
		int prev_mod;
	};

	QLinkedList<struct btn_node*> btn_list;
	QLinkedList<struct mod_data_node*> mod_data_list;

	// saved credential manager
	bool saved_credential_manager();

	// setter methods for setup class private variables.
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

	// setter methods for dev_controller class private variables.
	void set_d_id(string d_id);
	void set_status(string status);
	void set_range(string range);
	void set_mod_add(string mod_add);
	void set_pin(string pin);
	void set_device_controller_api_response(string resp);
	void set_device_controller_api_error_msg(string msg);

	// getter methods for setup class private variables.
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

	string get_user_credential_path();
	string get_mainwindow_bg_file_path();
	string get_dashboard_bg_file_path();
	string get_keyboad_icon_path();
	string get_update_icon_path();
	string get_settings_icon_path();
	string get_wifi_icon_path();
	string get_power_icon_path();
	string get_interface_up_script_path();

	// getter methods for dev_controller class private variables.
	string get_d_id();
	string get_status();
	string get_range();
	string get_mod_add();
	string get_pin();
	string get_device_controller_api_response();
	string get_device_controller_api_error_msg();

	int hex_to_int(string);
	string int_to_hex(int);
};

#endif // IALOY_DATA_H
