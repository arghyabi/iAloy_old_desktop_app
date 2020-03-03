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
#include <iostream>
#include <stdlib.h>

#include "connect_init.h"
#include "main.h"
#include "hardware.h"

using namespace std;

string ialoy_web_api::req_web_api(){

	try
	{
		//prepare session
		cout << this->req_url << endl;
		URI uri(this->req_url);
		HTTPClientSession session(uri.getHost(), uri.getPort());

		// prepare path
		string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";

		// send request
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.sendRequest(req);

		// get response
		HTTPResponse res;
		cout << res.getStatus() << " " << res.getReason() << endl;

		// print response
		istream &is = session.receiveResponse(res);

		// converting response from istream to string
		string data;
		char buffer[1024];
		while (is.read(buffer, sizeof(buffer)))
		{
			data.append(buffer, sizeof(buffer));
		}
		data.append(buffer, is.gcount());

		// return response
		return data;

	}catch(Exception &ex){
		return ex.displayText();
	}
}

string ialoy_web_api::fetch_pi_name()
{
	if(this->get_pi_add() == "")
	{
		this->set_pi_add();
	}
	this->req_url = this->url+"?aco=8&pi_add="+this->get_pi_add();
	return this->req_web_api();
}

string ialoy_web_api::check_email_pi_connection()
{
	this->set_pi_add();
	if((this->get_email() != "") && (this->get_pi_add() != ""))
	{
		this->req_url = this->url+"?aco=0&email="+this->get_email()+"&pi_add="+this->get_pi_add();
		return this->req_web_api();
	}
	else
	{
		string err_msg = "Email can't be blank.";
		return err_msg;
	}
}

string ialoy_web_api::pi_reg_status()
{
	if(this->get_pi_add() != "")
		this->req_url = this->url+"?aco=4&pi_add="+this->get_pi_add();
	return this->req_web_api();
}

bool ialoy_web_api::email_reg_status()
{
	this->req_url = this->url+"?aco=6&email="+this->get_email();
	if(this->req_web_api() == "1")
		return true;
	else
		return false;
}

string ialoy_web_api::fetch_user_details()
{
	this->req_url = this->url+"?aco=7&email="+this->get_email();
	return this->req_web_api();
}

string ialoy_web_api::check_product_id()
{
	if(this->get_product_id() != "")
	{
		this->req_url = this->url+"?aco=1&prod_id="+this->get_product_id();
		return this->req_web_api();
	}
	else
	{
		return "Product Id can't be blank.";
	}
}

int ialoy_web_api::login()
{
	if((this->get_email() != "") && (this->get_password() != ""))
	{
		this->req_url = this->url+"?aco=2&email="+this->get_email()+"&pi_add="+this->get_pi_add()+"&password="+this->get_password();
		string login_stat =  this->req_web_api();
		cout << "Login Stat: " << login_stat << endl;
		char* c = const_cast<char*>(login_stat.c_str());

		if(!strncmp(c, "1", 1))
		{
			cout << "Login success..." << endl;
			return true;
		}
		else
		{
			cout << "Login failed..." << endl;
			return false;
		}
	}
	else
	{
		string err_msg = "Email and password can't be blank.";
		return false;
	}
}

int ialoy_web_api::send_otp()
{
	this->req_url = this->url+"?aco=3&su_mail="+this->get_email();
	string send_otp_resp = this->req_web_api();
	char* c = const_cast<char*>(send_otp_resp.c_str());
	if(!strncmp(c, "1", 1))
	{
		this->set_otp(send_otp_resp.substr(1,send_otp_resp.length()-1));
		return 1;
	}
	else
		return 0;
}

int ialoy_web_api::reg_new_pi()
{
	if(this->get_user_type())
	{
		this->req_url = this->url+"?aco=5&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+ \
					"&pi_name="+this->get_pi_name()+"&f_name="+this->get_first_name()+"&l_name="+this->get_last_name()+"&password="+this->get_password();
		string reg_resp = this->req_web_api() ;
		if(reg_resp == "1")
			return 1;
		else
			return 0;
	}
	else
	{
		this->req_url = this->url+"?aco=5&type="+to_string(this->get_user_type())+"&prod_key="+this->get_product_id()+"&pi_add="+this->get_pi_add()+"&su_mail="+this->get_email()+ \
					"&pi_name="+this->get_pi_name();
		string reg_resp = this->req_web_api() ;
		if(reg_resp == "1")
			return 1;
		else
			return 0;
	}
}
