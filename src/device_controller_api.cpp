#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <QObject>

#include "device_controller_api.h"


void device_controller_api::set_device_controller_api_request(device_controller_api_request_type device_controller_api_request)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	device_controller_api_request_type_flag = device_controller_api_request;
	string dco = to_string(device_controller_api_request);
	this->set_device_controller_api_error_msg("");

	switch (device_controller_api_request)
	{
		case DEVICE_CONTROLLER_LOGIN_USING_TOKEN:
			{
				string login_url = "http://ialoy.arghyabiswas.com/desktop_api/set_up_req_mngr.php";

				this->req_url = login_url + "?aco=10&email=" + this->get_email() + "&token=" + QUrl::toPercentEncoding(this->get_token().c_str()).constData() + "&pi_add=" + this->get_pi_add();
				break;
			}

		case GET_DEVICE_DETAIL_STATUS:
			this->req_url = this->url+"?dco="+dco;
			break;

		case GET_ROOM_DEVICE_LIST:
			this->req_url = this->url+"?dco="+dco;
			break;

		case GET_ROOM_DEVICE_STATUS:
			this->req_url = this->url+"?dco="+dco;
			break;

		case UPDATE_STATUS:
			{
				if(this->get_d_id() != "" && this->get_status() != "")
					this->req_url = this->url+"?dco="+dco+"&d_id="+this->get_d_id()+"&status="+this->get_status();
				else
					this->set_device_controller_api_error_msg("Device id and Status can't be blank");
				break;
			}

		case UPDATE_RANGE:
			{
				if(this->get_d_id() != "" && this->get_range() != "")
					this->req_url = this->url+"?dco="+dco+"&d_id="+this->get_d_id()+"&range="+this->get_range();
				else
					this->set_device_controller_api_error_msg("Device id and Range can't be blank");
				break;
			}

		case GET_CONNECTED_PI_LIST:
			break;

		case UPDATE_STATUS_FOR_PI:
			{
				if(this->get_pin() != "" && this->get_mod_add() != "" && this->get_status() != "")
					this->req_url = this->url+"?dco="+dco+"&mod_add="+this->get_mod_add()+"&pin="+this->get_pin()+"&status="+this->get_status();
				else
					this->set_device_controller_api_error_msg("Module address, pin and Status can't be blank");
				break;
			}

		case UPDATE_RANGE_FOR_PI:
			{
				if(this->get_pin() != "" && this->get_mod_add() != "" && this->get_range() != "")
					this->req_url = this->url+"?dco="+dco+"&mod_add="+this->get_mod_add()+"&pin="+this->get_pin()+"&range="+this->get_range();
				else
					this->set_device_controller_api_error_msg("Module address, pin and Range can't be blank");
				break;
			}

		case GET_I2C_DATA:
			this->req_url = this->url+"?dco="+dco;
			break;

		case ADD_NEW_MODULE:
			{
				this->req_url = this->url+"?dco="+dco+"&mod_add="+this->get_mod_add();
			}

		default:
			break;
	}
	cout << this->req_url << endl;
	NetworkRequest.setUrl(QString::fromStdString(this->req_url));
}




