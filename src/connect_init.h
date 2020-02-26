#ifndef CONNECT_INIT_H
#define CONNECT_INIT_H

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class ialoy_web_api
{

private:

	string url = "http://ialoy.arghyabiswas.com/desktop_api/req_mngr.php";
	string req_url, pi_add, pi_name, email, product_id, password, f_name, l_name, phone;
	int reg_user_type;

public:

	string otp;

	// setter methods for private variables.
	void set_pi_add();
	void set_email(string email_id);
	void set_product_id(string prod_id);
	void set_password(string pass);
	void set_f_name(string f_name);
	void set_l_name(string l_name);
	void set_phone(string phone);
	void set_otp(string otp);
	void set_user_type(int type);
	void set_pi_name(string pi_name);

	string get_email();
	string get_product_id();

	// main operational functions.
	string req_web_api();
	string check_email_pi_connection();
	string check_product_id();
	int login();
	string pi_reg_status();
	bool email_reg_status();
	string get_user_details();
	int send_otp();
	int reg_new_pi();
};

#endif //CONNECT_INIT_H
