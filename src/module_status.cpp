#include "module_status.h"
#include "ui_module_status.h"

module_status::module_status(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::module_status)
{
	ui->setupUi(this);

	// QNetworkAccessManager *NetworkManager;
	// QNetworkRequest NetworkRequest;

	i2c_data *i2c_thread_obj = new i2c_data(this);
	connect(this, SIGNAL(read_all_i2c_module_state_signal(int*)), i2c_thread_obj, SLOT(read_all_i2c_module_state(int*)));
	// string url = "http://ialoy.arghyabiswas.com/desktop_api/device_controller_req_mngr.php?dco=2";
	// NetworkRequest.setUrl(QString::fromStdString(url));

	// NetworkManager = new QNetworkAccessManager();
	// QObject::connect(NetworkManager., &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
	// 		set_device_controller_api_response("");
	// 		if (reply->error())
	// 		{
	// 			cout << "Error : " << reply->errorString().toStdString() << endl;
	// 			ui->online_offline_label->setText("<b><font color='#FF4500'>Offline</font></b>");
	// 			return;
	// 		}
	// 		QString response = reply->readAll();
	// 		cout << "\ndevice controlelr api response : " << response.toStdString() << endl << endl;
	// 		//set_device_controller_api_response(response.toStdString());
	// 		render_module_status_status(response.toStdString());
	// 	}
	// );
}

module_status::~module_status()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
}

void module_status::button_clicked_slot(QString mod_add)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "buttton pressed: " << mod_add.toStdString() << endl;
}

// QJsonArray module_status::get_json_array_from_response(string response)
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	string room_device_state_data = response;
// 	QString room_device_status_info = QString::fromStdString(room_device_state_data);
// 	QJsonDocument jsonDocument = QJsonDocument::fromJson(room_device_status_info.toUtf8());
// 	QJsonObject jsonObject = jsonDocument.object();
// 	QJsonValue ArrayValue;
// 	ArrayValue = jsonObject.value("room_device_status");
// 	QJsonArray result_Array = ArrayValue.toArray();
// 	return result_Array;
// }

// bool module_status::device_controller_api_response_parse(string result)
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	string device_controller_api_res = result;
// 	if(device_controller_api_res.substr(0, 1) != "1")
// 		return false;

// 	device_controller_api_res = device_controller_api_res.substr(1, device_controller_api_res.length()-1);
// 	if(device_controller_api_res == "0Please Login first.")
// 	{
// 		cout << ">>>> " << __PRETTY_FUNCTION__ << "logout function called due to session failure" << endl;
// 		on_logout_button_clicked();
// 		return false;
// 	}
// 	return true;
// }

/*
Struct
{
	int mod_add;
	string room;
}

*/
// void module_status::render_module_status_state(string result)
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	if(device_controller_api_response_parse(result))
// 	{
// 		cout << "\n\nRender dashboard_room_btn_state data : " << get_device_controller_api_response() << endl << endl;
// 		QJsonArray roomArray = get_json_array_from_response(result);

// 		foreach (const QJsonValue &room, roomArray)
// 		{
// 			QJsonArray dev_array = room.toObject().value("dev_list").toArray();
// 			foreach (const QJsonValue &device, dev_array)
// 			{
// 				// setting up temp variables
// 				QString device_id = device.toObject().value("dev_id").toString();
// 				QString device_status_value = device.toObject().value("dev_status").toString();
// 				QString device_var_value = device.toObject().value("var_value").toString();

// 				struct btn_node *btn_nd;
// 				foreach(btn_nd, btn_list)
// 				{
// 					if(btn_nd->device_id == device_id)
// 					{
// 						if(device_status_value == "1")
// 						{
// 							btn_nd->btn_state = 1;
// 							// turning btn on
// 							btn_nd->btn->setStyleSheet("padding: 10%;background-color: #317AD7;color: #FFF;");
// 						}
// 						else
// 						{
// 							btn_nd->btn_state = 0;
// 							// turning btn off
// 							btn_nd->btn->setStyleSheet("padding: 10%;background-color: #525252;color: #FFF;");
// 						}
// 						// checking is_var enabled or not
// 						if(btn_nd->is_var)
// 						{
// 							btn_nd->slider->setValue(stoi(device_var_value.toStdString()));
// 							btn_nd->slider_val = stoi(device_var_value.toStdString());
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// }

void module_status::init()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	int col = 0;
	int row = 0;
	int status_array[117];

	emit read_all_i2c_module_state_signal(status_array);

	for (int index = 0; index < 117; index++)
	{
		// adding pushbutton to grid
		int int_mod_add = index+3;
		QString mod_add = QString::number(int_mod_add);
		btn = new QPushButton(mod_add);
		struct mod_data_node *mod_data_nd;
		bool isFound = false;

		foreach(mod_data_nd, mod_data_list)
		{
			cout << "mod add from ll:" << mod_data_nd->mod_add << " int_mod_add:" << int_mod_add << endl;
			if(mod_data_nd->mod_add == int_mod_add)
			{
				isFound = true;
				break;
			}
		}

		if(isFound == true)
		{
			if(status_array[index] == 1)
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #00FF00; color: #000;"));
			else if(status_array[index] == 0)
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #FF0000; color: #000;"));
		}
		else
		{
			if(status_array[index] == 1)
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #FFFF00; color: #000;"));
			else if(status_array[index] == 0)
				btn->setStyleSheet(QString::fromUtf8("height: 16px; padding: 5%; background-color: #525252; color: #000;"));
		}

		btn->setText(QString::fromStdString(int_to_hex(int_mod_add)));
		ui->status_grid_layout->addWidget(btn, row, col, 1,1);

		signalMapper = new QSignalMapper(this);
		signalMapper->setMapping(btn, mod_add);

		connect(btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
		connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(button_clicked_slot(QString)));

		if(col == 12)
		{
			col = 0;
			row++;
		}
		else
			col++;
	}
}