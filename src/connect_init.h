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

static string url = "http://ialoy.arghyabiswas.com/desktop_api/req_mngr.php";

string call_web_api(int input_stat, string email, string product_id);
string req_web_api(string url);


#endif //CONNECT_INIT_H
