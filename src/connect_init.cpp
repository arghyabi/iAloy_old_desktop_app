// #include <Poco/Net/HTTPClientSession.h>
// #include <Poco/Net/HTTPRequest.h>
// #include <Poco/Net/HTTPResponse.h>
// #include <Poco/StreamCopier.h>
// #include <Poco/Path.h>
// #include <Poco/URI.h>
// #include <Poco/Exception.h>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <QObject>

#include "connect_init.h"
#include "main.h"
#include "hardware.h"

using namespace std;

// string ialoy_web_api::req_web_api(){
// 	cout << this->req_url << endl;
// 	return this->req_url;
// 	// try
// 	// {
// 	// 	//prepare session
// 	// 	cout << this->req_url << endl;

// 	// 	// URI uri(this->req_url);
// 	// 	// HTTPClientSession session(uri.getHost(), uri.getPort());

// 	// 	// // prepare path
// 	// 	// string path(uri.getPathAndQuery());
// 	// 	// if (path.empty()) path = "/";

// 	// 	// // send request
// 	// 	// HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
// 	// 	// session.sendRequest(req);

// 	// 	// // get response
// 	// 	// HTTPResponse res;
// 	// 	// cout << res.getStatus() << " " << res.getReason() << endl;

// 	// 	// // print response
// 	// 	// istream &is = session.receiveResponse(res);

// 	// 	// // converting response from istream to string
// 	// 	// string data;
// 	// 	// char buffer[1024];
// 	// 	// while (is.read(buffer, sizeof(buffer)))
// 	// 	// {
// 	// 	// 	data.append(buffer, sizeof(buffer));
// 	// 	// }
// 	// 	// data.append(buffer, is.gcount());
// 	// 	string data;

// 	// 	// return response
// 	// 	// return data;
// 	// 	return "OK";
// 	// }
// 	// catch(Exception &ex){
// 	// 	return ex.displayText().toStdString();
// 	// }
// }

string ialoy_web_api::get_req_url()
{
	return this->req_url;
}


void ialoy_web_api::genarate_url(api_request_type genarate_url_flag)
{
	api_request = genarate_url_flag;
	string aco = to_string(genarate_url_flag);
	this->set_api_error_msg("");

	switch(genarate_url_flag)
	{
		case CHECK_PI_STATUS:
			if(this->get_pi_add() == "")
				this->set_pi_add();
			this->req_url = this->url+"?aco="+aco+"&pi_add="+this->get_pi_add();
			break;

		case GET_PI_NAME:
			if(this->get_pi_add() == "")
				this->set_pi_add();
			this->req_url = this->url+"?aco="+aco+"&pi_add="+this->get_pi_add();
			break;

		case CHECK_EMAIL_STATUS:
			if(this->get_email() != "")
				this->req_url = this->url+"?aco="+aco+"&email="+this->get_email();
			else
				this->set_api_error_msg("Email can't be blank...");
			break;

		case CHECK_EMAIL_CONNECTED_PI:
			if(this->get_pi_add() == "")
				this->set_pi_add();
			if(this->get_email() != "")
				this->req_url = this->url+"?aco="+aco+"&email="+this->get_email()+"&pi_add="+this->get_pi_add();
			else
				this->set_api_error_msg("Email can't be blank...");
			break;

		case GET_USER_DETAILS:
			if(this->get_email() != "")
				this->req_url = this->url+"?aco="+aco+"&email="+this->get_email();
			else
				this->set_api_error_msg("Email can't be blank...");
			break;

		case CHECK_PRODUCT_KEY_STATUS:
			if(this->get_product_id() != "")
				this->req_url = this->url+"?aco="+aco+"&prod_id="+this->get_product_id();
			else
				this->set_api_error_msg("Product Id can't be blank.");
			break;

		case SEND_OTP:
			if(this->get_email() != "")
				this->req_url = this->url+"?aco="+aco+"&su_mail="+this->get_email();
			else
				this->set_api_error_msg("Email can't be blank...");
			break;

		case REGISTER_NEW_PI:
			if(this->get_user_type())
			{
				if((this->get_email() != "") && (this->get_first_name() !="") && (this->get_last_name() != "") && (this->get_password() != "") && (this->get_pi_add() != "") && (this->get_pi_name() != "") && (this->get_product_id() != "") )
					this->req_url = this->url+"?aco="+aco+"&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+ \
							"&pi_name="+this->get_pi_name()+"&f_name="+this->get_first_name()+"&l_name="+this->get_last_name()+"&password="+this->get_password();
				else
					this->set_api_error_msg("One or more data(s) are missing...");
			}
			else
			{
				if((this->get_email() != "") && (this->get_pi_add() != "") && (this->get_pi_name() != "") && (this->get_product_id() != "") )
					this->req_url = this->url+"?aco="+aco+"&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+"&pi_name="+this->get_pi_name();
				else
					this->set_api_error_msg("One or more data(s) are missing...");
			}
			break;

		case LOGIN:
			if((this->get_email() != "") && (this->get_password() != ""))
				this->req_url = this->url+"?aco="+aco+"&email="+this->get_email()+"&pi_add="+this->get_pi_add()+"&password="+this->get_password();
			else
				this->set_api_error_msg("Email and password can't be blank.");
			break;

		case VERIFY_TOKEN:
			break;
		default:
			break;
	}

	cout << this->req_url << endl;
	NetworkRequest.setUrl(QString::fromStdString(this->req_url));
}

void ialoy_web_api::fetch_pi_name()
{
	this->set_api_error_msg("");
	if(this->get_pi_add() == "")
		this->set_pi_add();

	api_request = GET_PI_NAME;
	this->req_url = this->url+"?aco="+to_string(api_request)+"&pi_add="+this->get_pi_add();
}

void ialoy_web_api::check_email_pi_connection()
{
	this->set_api_error_msg("");
	if(this->get_pi_add() == "")
		this->set_pi_add();

	if(this->get_email() != "")
	{
		api_request = CHECK_EMAIL_CONNECTED_PI;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&email="+this->get_email()+"&pi_add="+this->get_pi_add();
	}
	else
	{
		this->set_api_error_msg("Email can't be blank...");
	}
}

void ialoy_web_api::pi_reg_status()
{
	this->set_api_error_msg("");
	if(this->get_pi_add() == "")
		this->set_pi_add();

	api_request = CHECK_PI_STATUS;
	this->req_url = this->url+"?aco="+to_string(api_request)+"&pi_add="+this->get_pi_add();
}

void ialoy_web_api::email_reg_status()
{
	this->set_api_error_msg("");

	if(this->get_email() != "")
	{
		api_request = CHECK_EMAIL_STATUS;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&email="+this->get_email();
	}
	else
	{
		this->set_api_error_msg("Email can't be blank...");
	}

}

void ialoy_web_api::fetch_user_details()
{
	this->set_api_error_msg("");

	if(this->get_email() != "")
	{
		api_request = GET_USER_DETAILS;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&email="+this->get_email();
	}
	else
	{
		this->set_api_error_msg("Email can't be blank...");
	}
}

void ialoy_web_api::check_product_id()
{
	this->set_api_error_msg("");

	if(this->get_product_id() != "")
	{
		api_request = CHECK_PRODUCT_KEY_STATUS;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&prod_id="+this->get_product_id();
	}
	else
	{
		this->set_api_error_msg("Product Id can't be blank.");
	}
}

void ialoy_web_api::login()
{
	this->set_api_error_msg("");

	if((this->get_email() != "") && (this->get_password() != ""))
	{
		api_request = LOGIN;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&email="+this->get_email()+"&pi_add="+this->get_pi_add()+"&password="+this->get_password();

		// string login_stat =  this->req_web_api();
		// cout << "Login Stat: " << login_stat << endl;
		// char* c = const_cast<char*>(login_stat.c_str());

		// if(!strncmp(c, "1", 1))
		// {
		// 	cout << "Login success..." << endl;
		// 	return true;
		// }
		// else
		// {
		// 	cout << "Login failed..." << endl;
		// 	return false;
		// }
	}
	else
	{
		this->set_api_error_msg("Email and password can't be blank.");
	}
}

void ialoy_web_api::send_otp()
{
	this->set_api_error_msg("");

	if(this->get_email() != "")
	{
		api_request = SEND_OTP;
		this->req_url = this->url+"?aco="+to_string(api_request)+"&su_mail="+this->get_email();
	}
	else
	{
		this->set_api_error_msg("Email can't be blank...");
	}

	// string send_otp_resp = this->req_web_api();
	// char* c = const_cast<char*>(send_otp_resp.c_str());
	// if(!strncmp(c, "1", 1))
	// {
	// 	this->set_otp(send_otp_resp.substr(1,send_otp_resp.length()-1));
	// 	return 1;
	// }
	// else
	// 	return 0;
}

void ialoy_web_api::reg_new_pi()
{
	this->set_api_error_msg("");

	if(this->get_user_type())
	{
		if((this->get_email() != "") && (this->get_first_name() !="") && (this->get_last_name() != "") && (this->get_password() != "") && (this->get_pi_add() != "") && (this->get_pi_name() != "") && (this->get_product_id() != "") )
		{
			api_request = REGISTER_NEW_PI;
			this->req_url = this->url+"?aco="+to_string(api_request)+"&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+ \
					"&pi_name="+this->get_pi_name()+"&f_name="+this->get_first_name()+"&l_name="+this->get_last_name()+"&password="+this->get_password();
		}
		else
		{
			this->set_api_error_msg("One or more data(s) are missing...");
		}

		// string reg_resp = this->req_web_api() ;
		// if(reg_resp == "1")
		// 	return 1;
		// else
		// 	return 0;
	}
	else
	{
		if((this->get_email() != "") && (this->get_pi_add() != "") && (this->get_pi_name() != "") && (this->get_product_id() != "") )
		{
			api_request = REGISTER_NEW_PI;
			this->req_url = this->url+"?aco="+to_string(api_request)+"&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+"&pi_name="+this->get_pi_name();
		}
		else
		{
			this->set_api_error_msg("One or more data(s) are missing...");
		}
	}
}
