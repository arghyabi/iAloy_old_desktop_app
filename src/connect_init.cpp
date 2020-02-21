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

void ialoy_web_api::set_pi_add()
{
	this->pi_add = get_serial();
	cout << "Pi address : " << this->pi_add << endl;
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
