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

#include "ialoy_data.h"

using namespace Poco::Net;
using namespace Poco;
using namespace std;

class ialoy_web_api : public ialoy_main_data
{

private:
	string protocol = "http://";
	string sub_domain = "ialoy";
	string domain = "arghyabiswas.com";
	string api = "desktop_api";
	string req_manager = "req_mngr.php";
	string url = protocol+sub_domain+"."+domain+"/"+api+"/"+req_manager;
	string req_url;

public:
	// main operational functions.
	string req_web_api();
	string fetch_pi_name();
	string check_email_pi_connection();
	string check_product_id();
	int login();
	string pi_reg_status();
	bool email_reg_status();
	string fetch_user_details();
	int send_otp();
	int reg_new_pi();

};

#endif //CONNECT_INIT_H
