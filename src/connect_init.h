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

class ialoy_web_api{

private:

	string url = "http://ialoy.arghyabiswas.com/desktop_api/req_mngr.php";
	string req_url, pi_add, email, product_id, password;

public:

	// setter methods for private variables.
	void set_pi_add();
	void set_email(string email_id);
	void set_product_id(string prod_id);
	void set_password(string pass);

	// main operational functions.
	string req_web_api();
	string check_email_pi_connection();
	string check_product_id();
	string login();

};

#endif //CONNECT_INIT_H
