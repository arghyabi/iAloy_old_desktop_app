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
#include "hardware.h"

using namespace std;

void ialoy_web_api::set_email(string email_id)
{
	this->email = email_id;
	cout << "Email : " << this->email << endl;
}

void ialoy_web_api::set_product_id(string prod_id)
{
	this->product_id = prod_id;
}

void ialoy_web_api::set_password(string pass)
{
	this->password = pass;
}

void ialoy_web_api::set_f_name(string f_name)
{
	this->f_name = f_name;
}

void ialoy_web_api::set_l_name(string l_name)
{
	this->l_name = l_name;
}

void ialoy_web_api::set_phone(string phone)
{
	this->phone = phone;
}

void ialoy_web_api::set_otp(string otp)
{
	this->otp = otp;
}

void ialoy_web_api::set_pi_add()
{
	this->pi_add = get_serial();
	cout << "Pi address : " << this->pi_add << endl;
}

string ialoy_web_api::get_email()
{
	return this->email;
}

string ialoy_web_api::get_product_id()
{
	return this->product_id;
}

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

string ialoy_web_api::get_pi_name()
{
	if(this->pi_add == "")
	{
		this->set_pi_add();
	}
	this->req_url = this->url+"?aco=8&pi_add="+this->pi_add;
	return this->req_web_api();
}

string ialoy_web_api::check_email_pi_connection()
{
	this->set_pi_add();
	if((this->email != "") && (this->pi_add != ""))
	{
		this->req_url = this->url+"?aco=0&email="+this->email+"&pi_add="+this->pi_add;
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
	if(this->pi_add != "")
		this->req_url = this->url+"?aco=4&pi_add="+this->pi_add;
	return this->req_web_api();
}

bool ialoy_web_api::email_reg_status()
{
	this->req_url = this->url+"?aco=6&email="+this->email;
	if(this->req_web_api() == "1")
		return true;
	else
		return false;
}

string ialoy_web_api::get_user_details()
{
	this->req_url = this->url+"?aco=7&email="+this->email;
	return this->req_web_api();
}

string ialoy_web_api::check_product_id()
{
	if(this->product_id != "")
	{
		this->req_url = this->url+"?aco=1&prod_id="+this->product_id;
		return this->req_web_api();
	}
	else
	{
		string err_msg = "Product Id can't be blank.";
		return err_msg;
	}
}

int ialoy_web_api::login()
{
	if((this->email != "") && (this->password != ""))
	{
		this->req_url = this->url+"?aco=2&email="+this->email+"&pi_add="+this->pi_add+"&password="+this->password;
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
		//return login_stat;
	}
	else
	{
		string err_msg = "Email and password can't be blank.";
		return false;
	}
}

int ialoy_web_api::send_otp()
{
	this->req_url = this->url+"?aco=3&su_mail="+this->email;
	string send_otp_resp = this->req_web_api();
	char* c = const_cast<char*>(send_otp_resp.c_str());
	if(!strncmp(c, "1", 1))
	{
		this->otp = send_otp_resp.substr(1,send_otp_resp.length()-1);
		return 1;
	}
	else
		return 0;
}

void ialoy_web_api::set_user_type(int type)
{
	this->reg_user_type = type;
}

void ialoy_web_api::set_pi_name(string piName)
{
	this->pi_name = piName;
}

int ialoy_web_api::reg_new_pi()
{
	if(this->reg_user_type)
	{
		this->req_url = this->url+"?aco=5&type="+to_string(this->reg_user_type)+"&prod_key="+this->product_id+"&pi_add="+this->pi_add+"&su_mail="+this->email+ \
					"&pi_name="+this->pi_name+"&f_name="+this->f_name+"&l_name="+this->l_name+"&password="+this->password;
		string reg_resp = this->req_web_api() ;	
		if(reg_resp == "1")
			return 1;
		else
			return 0;
	}
	else
	{
		this->req_url = this->url+"?aco=5&type="+to_string(this->reg_user_type)+"&prod_key="+this->product_id+"&pi_add="+this->pi_add+"&su_mail="+this->email+ \
					"&pi_name="+this->pi_name;
		string reg_resp = this->req_web_api() ;	
		if(reg_resp == "1")
			return 1;
		else
			return 0;
	}
}

