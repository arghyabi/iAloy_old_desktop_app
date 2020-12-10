#ifndef CONNECT_INIT_H
#define CONNECT_INIT_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <iostream>
#include <string>
#include <fstream>
#include <QObject>
#include <QFile>

#include "ialoy_data.h"

using namespace std;

class ialoy_web_api : public ialoy_main_data
{

private:
	string protocol = "https://";
	string sub_domain = "ialoy";
	string domain = "arghyabiswas.com/";
	string req_manager = "login_manager";
	string url = protocol+sub_domain+"."+domain+req_manager;
	string req_url;

public:
	// main operational functions.
	void set_api_request(api_request_type genarate_url_flag);
	void store_user_credential();
	string get_req_url();

};

#endif //CONNECT_INIT_H
