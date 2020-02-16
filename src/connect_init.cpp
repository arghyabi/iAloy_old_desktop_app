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

#include "connect_init.h"

using namespace std;

string call_web_api(int input_stat, string email, string product_id = ""){

	string pi_id = "456";

	if(input_stat == 0){
		url += "?aco="+to_string(input_stat)+"&email="+email+"&pi_id="+pi_id;
		cout << "URL : " << url << endl;
		string response = req_web_api(url);
		return response;
	}else if(input_stat == 1){
		url += "?email="+email+"&pi_id="+pi_id+"&prod_id="+product_id;
		cout << "URL : " << url << endl;
		string response = req_web_api(url);
		return response;
	}else{
		cout << "URL : " << url << endl;
		string response = req_web_api(url);
		return response;
	}

}

string req_web_api(string url){

	try
	{
		/* code */
		//prepare session
		URI uri(url);
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
		string err_msg = ex.displayText();
		return err_msg;
	}

}

