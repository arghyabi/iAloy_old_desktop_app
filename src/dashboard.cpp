#include "dashboard.h"
#include "ui_dashboard.h"

#include <QDebug>

using namespace std;

dashboard::dashboard(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::dashboard)
{
	ui->setupUi(this);

	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	ui->online_offline_label->setText("<b><font color='#000000'>Connecting...</font></b>");
	ui->ialoy_logo_label->setText("<b><font size=8 color='#1e93b6'>i</font><font size=8 color='#555'>Aloy</font></b>");
	ui->ialoy_tag_line_label->setText("<font size=4 color='#415c76'><b>Smart Home for Smart Future</b></font>");
	timer_for_datetime = new QTimer(this);
	timer_for_i2c_data_read_from_web = new QTimer(this);
	connect(timer_for_datetime, SIGNAL(timeout()), this, SLOT(update_time()));
	connect(timer_for_i2c_data_read_from_web, SIGNAL(timeout()), this, SLOT(get_i2c_web_status()));
	timer_for_datetime->start(1000);

	ui->settings_tool_button->setIcon(QIcon(QString::fromStdString(dashboard::get_settings_icon_path())));
	ui->wifi_tool_button->setIcon(QIcon(QString::fromStdString(dashboard::get_wifi_icon_path())));
	ui->keyboard_tool_button->setIcon(QIcon(QString::fromStdString(dashboard::get_keyboad_icon_path())));
	ui->app_update_button->setIcon(QIcon(QString::fromStdString(dashboard::get_update_icon_path())));
	ui->power_tool_button->setIcon(QIcon(QString::fromStdString(dashboard::get_power_icon_path())));

	i2c_data *i2c_thread_obj = new i2c_data(this);
	i2c_thread_obj->moveToThread(&i2cWorkerThread);
	connect(this, SIGNAL(send_i2c_data_to_module_signal(int, int)), i2c_thread_obj, SLOT(write_i2c_data(int, int)));
	connect(this, SIGNAL(read_request_i2c_data_from_module_signal(int, int)), i2c_thread_obj, SLOT(read_i2c_data(int, int)));
	connect(i2c_thread_obj, SIGNAL(receive_i2c_data_from_module(int, int)), this, SLOT(read_i2c_data_from_module(int, int)));

	timer_for_i2c_data_read_from_mod = new QTimer(this);
	connect(timer_for_i2c_data_read_from_mod, SIGNAL(timeout()), this, SLOT(read_request_i2c_data_from_module()));
}

void dashboard::update_time()
{
	QDateTime dateTime = dateTime.currentDateTime();
	QString timeString = dateTime.toString("hh:mm:ss ap");
	QString dateString = dateTime.toString("dd-MMMM-yyyy");
	ui->time_label->setText("<font size=4><b>"+timeString+"</b></font><br/>  "+dateString);
}

// init method contain network->get()
void dashboard::init()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	DBNetworkManager = new QNetworkAccessManager();
	QObject::connect(DBNetworkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
			set_device_controller_api_response("");
			if (reply->error())
			{
				cout << "Error : " << reply->errorString().toStdString() << endl;
				ui->online_offline_label->setText("<b><font color='#FF4500'>Offline</font></b>");
				return;
			}
			QString response = reply->readAll();
			cout << "\ndevice controlelr api response : " << response.toStdString() << endl << endl;
			set_device_controller_api_response(response.toStdString());
			update_dashboard_gui();
		}
	);

	if(dashboard::saved_credential_manager())
	{
		set_device_controller_api_request(DEVICE_CONTROLLER_LOGIN_USING_TOKEN);
		send_device_controller_api_request();
	}
	else
	{
		main_window_show(true);
		dashboard_window_show(false);
	}
}

// contain calling statement of network->get()
void dashboard::send_device_controller_api_request()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(this->get_device_controller_api_error_msg() == "")
		DBNetworkManager->get(NetworkRequest);
	else
	{
		cout << "Error massage : " << this->get_device_controller_api_error_msg() << endl;
	}
}

// update_dashboard_gui method to call specific render method
// wrt device_controller_api_request_type
void dashboard::update_dashboard_gui()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "DB-Flag:" << device_controller_api_request_type_flag << endl;
	switch(device_controller_api_request_type_flag)
	{
		case DEVICE_CONTROLLER_LOGIN_USING_TOKEN:
			timer_for_i2c_data_read_from_web->start(1000);
			render_dashboard_login();
			break;

		case GET_DEVICE_DETAIL_STATUS:
			// render device_detail_status
			break;

		case GET_ROOM_DEVICE_LIST:
			// render room_device_list
			cout << "Entering render_dashboard_room_btn() " << endl;
			render_dashboard_room_btn();
			break;

		case GET_ROOM_DEVICE_STATUS:
			// render room_device_status
			cout << "Entering render_dashboard_room_btn_status() " << endl;
			render_dashboard_room_btn_state();
			break;

		case UPDATE_STATUS:
			// render update status
			break;

		case UPDATE_RANGE:
			// render update range
			break;

		case GET_CONNECTED_PI_LIST:
			break;

		case UPDATE_STATUS_FOR_PI:
			// render update_status_for_pi
			break;

		case UPDATE_RANGE_FOR_PI:
			// render update_range_for_pi
			break;

		case GET_I2C_DATA:
			// render i2c data
			render_i2c_data();
			break;

		default:
			break;
	}
}

// render after dashboard_saved_login response
void dashboard::render_dashboard_login()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(device_controller_api_response_parse() && this->get_device_controller_api_response().substr(0,1) == "1")
	{
		cout << "DB-LOGIN STATUS : Success" << endl;
		ui->pi_name_label->setText(QString::fromStdString("<center><b><font size=5>"+this->get_pi_name()+"</font></b><br/>"+this->get_email()+"</center>"));
		set_device_controller_api_request(GET_ROOM_DEVICE_LIST);
		send_device_controller_api_request();
	}
	else
	{
		cout << "Response : " << this->get_device_controller_api_response() << endl;
		cout << "DB-LOGIN STATUS : Fail... Redirecting to MainWindow" << endl;
		main_window_show(true);
		dashboard_window_show(false);
	}
}

bool dashboard::device_controller_api_response_parse()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string device_controller_api_res = this->get_device_controller_api_response();
	if(device_controller_api_res.substr(0, 1) != "1")
		return false;

	device_controller_api_res = device_controller_api_res.substr(1, device_controller_api_res.length()-1);
	if(device_controller_api_res == "0Please Login first.")
	{
		cout << ">>>> " << __PRETTY_FUNCTION__ << "logout function called due to session failure" << endl;
		on_logout_button_clicked();
		return false;
	}
	this->set_pi_name(device_controller_api_res.substr(1, device_controller_api_res.length()-1));
	cout << "Pi_name from dashboard : " << this->get_pi_name() << endl;

	this->set_device_controller_api_response(device_controller_api_res);

	return true;
}

QJsonArray dashboard::get_json_array_from_response()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string room_device_state_data = this->get_device_controller_api_response();
	QString room_device_status_info = QString::fromStdString(room_device_state_data);
	QJsonDocument jsonDocument = QJsonDocument::fromJson(room_device_status_info.toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QJsonValue ArrayValue;
	switch(this->device_controller_api_request_type_flag)
	{
		case GET_ROOM_DEVICE_LIST:
			ArrayValue = jsonObject.value("room_device_details");
			break;

		case GET_ROOM_DEVICE_STATUS:
			ArrayValue = jsonObject.value("room_device_status");
			break;

		case GET_I2C_DATA:
			ArrayValue = jsonObject.value("i2c_data");
			break;
	}

	QJsonArray result_Array = ArrayValue.toArray();
	return result_Array;
}

void dashboard::clearLayout(QLayout* layout)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	while (QLayoutItem* item = layout->takeAt(0))
	{
		if (QWidget* widget = item->widget())
			widget->deleteLater();
		if (QLayout* childLayout = item->layout())
			clearLayout(childLayout);
		delete item;
	}
}

void dashboard::render_dashboard_room_btn()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(this->device_controller_api_response_parse())
	{
		// store response in structure_linked_list here
		struct btn_node *tmp_btn_nd;
		if(!btn_list.isEmpty())
		{
			clearLayout(ui->v_layout_for_all_room);
			btn_list.clear();
		}

		QJsonArray roomArray = get_json_array_from_response();

		// creating main_room_container
		v_layout_for_all_room = new QVBoxLayout();

		foreach (const QJsonValue &room, roomArray)
		{
			h_layout_for_room_and_spacer = new QHBoxLayout();
			v_layout_for_all_btn_node_and_room_label = new QVBoxLayout();
			h_layout_for_all_btn_node = new QHBoxLayout();
			room_name_label = new QLabel();
			spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

			QString room_name = room.toObject().value("room_name").toString();
			room_name_label->setText(room_name);

			v_layout_for_all_btn_node_and_room_label->addWidget(room_name_label);

			QJsonArray dev_array = room.toObject().value("dev_list").toArray();

			foreach (const QJsonValue &device, dev_array)
			{
				tmp_btn_nd = new struct btn_node;

				QString device_name = device.toObject().value("d_name").toString();
				QString device_id = device.toObject().value("dev_id").toString();
				QString device_is_var = device.toObject().value("is_var").toString();

				tmp_btn_nd->pin_num = device.toObject().value("pin").toInt();
				tmp_btn_nd->mod_add = hex_to_int(device.toObject().value("mod_add").toString().toStdString());

				tmp_btn_nd->btn = new QPushButton(device_id);
				tmp_btn_nd->slider = new QSlider();
				v_layout_for_btn_node = new QVBoxLayout();
				signalMapper = new QSignalMapper(this);
				signalMapper->setMapping(tmp_btn_nd->btn, device_id);

				tmp_btn_nd->device_id = device_id;

				QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
				sizePolicy.setHorizontalStretch(0);
				sizePolicy.setVerticalStretch(0);
				sizePolicy.setHeightForWidth(tmp_btn_nd->btn->sizePolicy().hasHeightForWidth());
				tmp_btn_nd->btn->setSizePolicy(sizePolicy);
				tmp_btn_nd->btn->setMinimumSize(QSize(0, 0));
				tmp_btn_nd->btn->setText(device_name);

				connect(tmp_btn_nd->btn, SIGNAL(clicked()), signalMapper, SLOT(map()));
				connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(button_clicked_slot(QString)));

				sizePolicy.setHeightForWidth(tmp_btn_nd->slider->sizePolicy().hasHeightForWidth());
				tmp_btn_nd->slider->setSizePolicy(sizePolicy);
				tmp_btn_nd->slider->setOrientation(Qt::Horizontal);

				if(device_is_var == "1")
				{
					tmp_btn_nd->slider->setEnabled(true);
					tmp_btn_nd->is_var = true;
				}
				else
				{
					tmp_btn_nd->slider->setEnabled(false);
					tmp_btn_nd->is_var = false;
					// tmp_btn_nd->slider->setVisible(false);
				}

				btn_list.append(tmp_btn_nd);

				v_layout_for_btn_node->addWidget(tmp_btn_nd->btn);
				v_layout_for_btn_node->addWidget(tmp_btn_nd->slider);
				h_layout_for_all_btn_node->addLayout(v_layout_for_btn_node);
			}

			v_layout_for_all_btn_node_and_room_label->addLayout(h_layout_for_all_btn_node);
			h_layout_for_room_and_spacer->addLayout(v_layout_for_all_btn_node_and_room_label);
			h_layout_for_room_and_spacer->addItem(spacerItem);
			ui->v_layout_for_all_room->addLayout(h_layout_for_room_and_spacer);
		}

		cout << "------------- Room device render success --------------------" << endl;
		set_device_controller_api_request(GET_ROOM_DEVICE_STATUS);
		send_device_controller_api_request();
	}
}

void dashboard::render_dashboard_room_btn_state()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	// render using dco = 3 data
	if(device_controller_api_response_parse())
	{
		cout << "\n\nRender dashboard_room_btn_state data : " << get_device_controller_api_response() << endl << endl;
		QJsonArray roomArray = get_json_array_from_response();
		foreach (const QJsonValue &room, roomArray)
		{
			QJsonArray dev_array = room.toObject().value("dev_list").toArray();

			foreach (const QJsonValue &device, dev_array)
			{
				// setting up temp variables
				QString device_id = device.toObject().value("dev_id").toString();
				QString device_status_value = device.toObject().value("dev_status").toString();
				QString device_var_value = device.toObject().value("var_value").toString();

				struct btn_node *btn_nd;
				foreach(btn_nd, btn_list)
				{
					if(btn_nd->device_id == device_id)
					{
						if(device_status_value == "1")
							btn_nd->btn_state = 1;
						else
							btn_nd->btn_state = 0;

						if(btn_nd->btn_state)
						{
							// turning btn on
							btn_nd->btn->setStyleSheet("background-color: #74a6f3; color: #000;");
						}
						else
						{
							// turning btn off
							btn_nd->btn->setStyleSheet("background-color:  #525252; color: #fff;");
						}

						// checking is_var enabled or not
						if(btn_nd->is_var)
						{
							// setting var_value
							btn_nd->slider->setValue(stoi(device_var_value.toStdString()));
							btn_nd->slider_val = stoi(device_var_value.toStdString());
						}
						else
							btn_nd->slider->setDisabled(1);
					}
				}
			}
		}
		cout << "------------- state render success --------------------" << endl;

		set_device_controller_api_request(GET_I2C_DATA);
		send_device_controller_api_request();
	}
}

void dashboard::button_clicked_slot(QString device_id)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << " button id: " << device_id.toStdString() << " is clicked." << endl;
	struct btn_node *btn_nd;
	int data_to_send = 0;
	int tmp = 1;
	int mod_add;
	foreach(btn_nd, btn_list)
	{
		if(btn_nd->device_id == device_id)
		{
			mod_add = btn_nd->mod_add;
			if(btn_nd->btn_state == 1)
			{
				cout << "found the " << device_id.toStdString() << " button; current is 1, will be 0" << endl;
				btn_nd->btn_state = 0;
				btn_nd->btn->setStyleSheet("background-color: #FF33E9; color:  #000;");
			}
			else
			{
				cout << "found the " << device_id.toStdString() << " button; current is 0, will be 1" << endl;
				btn_nd->btn_state = 1;
				btn_nd->btn->setStyleSheet("background-color:  #0DD8B3; color:  #fff;");
			}
			this->set_d_id(device_id.toStdString());
			this->set_status(to_string(btn_nd->btn_state));
		}
	}

	foreach(btn_nd, btn_list)
	{
		if(btn_nd->mod_add == mod_add)
		{
			if(btn_nd->btn_state == 1)
				data_to_send += tmp;
			tmp *= 2;
		}
	}

	emit send_i2c_data_to_module_signal(mod_add, data_to_send);

	set_device_controller_api_request(UPDATE_STATUS);
	send_device_controller_api_request();
}

void dashboard::get_i2c_web_status()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	set_device_controller_api_request(GET_I2C_DATA);
	send_device_controller_api_request();
}

void dashboard::render_i2c_data()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(device_controller_api_response_parse())
	{
		ui->online_offline_label->setText("<b><font color='#228B22'>Online</font></b>");
		timer_for_i2c_data_read_from_mod->start(1000);
		string tmp_api_resp = dashboard::get_device_controller_api_response();
		cout << "\n\nI2C data : \n" << tmp_api_resp << endl << endl;
		// checking if device state is updated or not
		if(api_i2c_data == "" || api_i2c_data != tmp_api_resp)
		{
			// set updated device state in temp variable
			api_i2c_data = tmp_api_resp;
			QJsonArray i2c_array = get_json_array_from_response();

			int data_to_send = 0;
			int mod_Add;

			struct mod_data_node *mod_data_nd;
			mod_data_nd = new struct mod_data_node;
			mod_data_nd->mod_add = 0;
			mod_data_nd->current_web = 0;
			mod_data_nd->prev_web = 0;
			mod_data_nd->current_mod = 0;
			mod_data_nd->prev_mod = 0;

			if(!mod_data_list.isEmpty())
				mod_data_list.clear();

			//assign value
			foreach(const QJsonValue mod_info, i2c_array)
			{
				mod_Add = hex_to_int(mod_info.toObject().value("mod_add").toString().toStdString());

				QJsonArray status_array = mod_info.toObject().value("pin_status").toArray();

				int tmp = 1;
				for(int i = 0; i < 8; i++)
				{
					if(status_array[i] == 1)
						data_to_send += tmp;
					tmp *= 2;
				}

				cout << "\n\nMod address : " << mod_Add << "\nStatus Code : " << data_to_send << endl;
				mod_data_nd->mod_add = mod_Add;
				mod_data_nd->prev_web = mod_data_nd->current_web;
				mod_data_nd->current_web = data_to_send;

				mod_data_list.append(mod_data_nd);

				// if(mod_data_nd->current_mod != mod_data_nd->current_web)
				// {	//ERROR: ervdery time the current_mod is 0 due to fresh initilization
					cout << "current mod: " << mod_data_nd->current_mod << "\t current_web: " << mod_data_nd->current_web << endl;
					emit send_i2c_data_to_module_signal(mod_Add, data_to_send);
				//}
				data_to_send = 0;
			}

			// get_room_device_status api call
			if(api_i2c_data.length() == tmp_api_resp.length())
			{
				set_device_controller_api_request(GET_ROOM_DEVICE_STATUS);
				send_device_controller_api_request();
			}
			else
			{
				set_device_controller_api_request(GET_ROOM_DEVICE_LIST);
				send_device_controller_api_request();
			}
		}
	}
}

dashboard::~dashboard()
{
	delete ui;
}

void dashboard::read_request_i2c_data_from_module()
{
	//This function is getting called by timer in every 1 second
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	struct mod_data_node *mod_data_nd;
	foreach(mod_data_nd, mod_data_list)
		emit read_request_i2c_data_from_module_signal(mod_data_nd->mod_add, mod_data_nd->current_mod);
}

void dashboard::read_i2c_data_from_module(int mod_address, int received_data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	struct mod_data_node *mod_data_nd;
	struct btn_node *btn_nd;
	foreach(mod_data_nd, mod_data_list)
	{
		if(mod_data_nd->mod_add == mod_address)
		{
			mod_data_nd->prev_mod = mod_data_nd->current_mod;
			mod_data_nd->current_mod = received_data;
			if(mod_data_nd->current_mod != mod_data_nd->current_web)
			{
				int xor_res = mod_data_nd->current_mod ^ mod_data_nd->prev_mod;
				int pin = 0;
				int tmp;
				//TODO:need 8 times loop
				do
				{
					tmp = xor_res & 1;
					xor_res = xor_res >> 1;
					pin++;
				}
				while(!tmp);

				int pin_state = (received_data >> (pin - 1)) & 1;

				cout << "Switch state changed... \tPin : " << pin << " state: " << pin_state << endl;

				foreach(btn_nd, btn_list)
				{
					if(btn_nd->mod_add == mod_address && btn_nd->pin_num == pin)
					{
						if(pin_state == 1)
						{
							btn_nd->btn_state = 1;
							btn_nd->btn->setStyleSheet("background-color: #00FF00; color:  #000;");
						}
						else
						{
							btn_nd->btn_state = 0;
							btn_nd->btn->setStyleSheet("background-color:  #FF0000; color:  #fff;");
						}
						break;
					}
				}
				// TODO : int to hex
				this->set_mod_add("0x04");
				this->set_pin(to_string(pin));
				this->set_status(to_string(pin_state));
				set_device_controller_api_request(UPDATE_STATUS_FOR_PI);
				send_device_controller_api_request();
				break;
			}
		}
	}
}

void dashboard::addBgImage()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QPixmap bkgnd("/usr/share/iAloy/photos/dashboard.jpg");
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

int dashboard::hex_to_int(string hex)
{
	int int_data = 0;
	int tmp = 1;

	for(int i = 2; i < 4; i++)
	{
		switch(hex[i])
		{
			case '0':
				int_data = tmp * int_data;
				break;
			case '1':
				int_data = tmp * int_data + 1;
				break;
			case '2':
				int_data = tmp * int_data + 2;
				break;
			case '3':
				int_data = tmp * int_data + 3;
				break;
			case '4':
				int_data = tmp * int_data + 4;
				break;
			case '5':
				int_data = tmp * int_data + 5;
				break;
			case '6':
				int_data = tmp * int_data + 6;
				break;
			case '7':
				int_data = tmp * int_data + 7;
				break;
			case '8':
				int_data = tmp * int_data + 8;
				break;
			case '9':
				int_data = tmp * int_data + 9;
				break;
			case 'a':
			case 'A':
				int_data = tmp * int_data + 10;
				break;
			case 'b':
			case 'B':
				int_data = tmp * int_data + 11;
				break;
			case 'c':
			case 'C':
				int_data = tmp * int_data + 12;
				break;
			case 'd':
			case 'D':
				int_data = tmp * int_data + 13;
				break;
			case 'e':
			case 'E':
				int_data = tmp * int_data + 14;
				break;
			case 'f':
			case 'F':
				int_data = tmp * int_data + 15;
				break;
		}
		tmp *= 10;
	}

	return int_data;
}

void dashboard::on_power_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
#ifdef ARC_TYPE
	system("reboot");
#else
	this->close();
#endif
}

void dashboard::on_settings_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	settings_window_show(true);
}

void dashboard::on_wifi_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

void dashboard::on_keyboard_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

void dashboard::on_app_update_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	update_manager_window_show(true);
}

void dashboard::on_logout_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	system("mv /usr/share/iAloy/.conf/credential.json /usr/share/iAloy/.conf/credential_old.json");
	timer_for_i2c_data_read_from_web->stop();
	timer_for_i2c_data_read_from_mod->stop();
	main_window_show(true);
	dashboard_window_show(false);
	mainwindow_reset_on_logout();
}
