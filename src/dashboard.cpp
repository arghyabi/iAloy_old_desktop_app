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
}

// contain calling statement of network->get()
void dashboard::send_device_controller_api_request()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(this->get_device_controller_api_error_msg() == "")
		DBNetworkManager->get(NetworkRequest);
	else
		cout << "Error massage : " << this->get_device_controller_api_error_msg() << endl;
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
	if(device_controller_api_response_parse() && this->get_device_controller_api_response() == "1")
	{
		cout << "DB-LOGIN STATUS : Success" << endl;
		set_device_controller_api_request(GET_ROOM_DEVICE_LIST);
		send_device_controller_api_request();
	}
	else
		cout << "DB-LOGIN STATUS : Success" << endl;
}


bool dashboard::device_controller_api_response_parse()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string device_controller_api_res = this->get_device_controller_api_response();
	if(device_controller_api_res.substr(0, 1) != "1")
		return false;

	device_controller_api_res = device_controller_api_res.substr(1, device_controller_api_res.length()-1);

	this->set_device_controller_api_response(device_controller_api_res);

	return true;
}

QJsonArray dashboard::get_json_array_from_response()
{
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
	}

	QJsonArray result_Array = ArrayValue.toArray();
	return result_Array;
}


void dashboard::render_dashboard_room_btn()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	if(this->device_controller_api_response_parse())
	{
		// store response in structure_linked_list here
		struct btn_node *tmp_btn_nd;

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
				tmp_btn_nd->btn = new QPushButton();
				tmp_btn_nd->slider = new QSlider();
				v_layout_for_btn_node = new QVBoxLayout();

				QString device_name = device.toObject().value("d_name").toString();
				QString device_id = device.toObject().value("dev_id").toString();
				QString device_is_var = device.toObject().value("is_var").toString();

				tmp_btn_nd->device_id = device_id;

				QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
				sizePolicy.setHorizontalStretch(0);
				sizePolicy.setVerticalStretch(0);
				sizePolicy.setHeightForWidth(tmp_btn_nd->btn->sizePolicy().hasHeightForWidth());
				tmp_btn_nd->btn->setSizePolicy(sizePolicy);
				tmp_btn_nd->btn->setMinimumSize(QSize(0, 0));
				tmp_btn_nd->btn->setText(device_name);

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
						{
							// turning btn on
							btn_nd->btn->setStyleSheet("background-color: #74a6f3; color:  #ffffff;");
						}
						else if(device_status_value == "0")
						{
							// turning btn off
							btn_nd->btn->setStyleSheet("background-color:  #525252; color:  #ffffff;");
						}

						// checking is_var enabled or not
						if(btn_nd->is_var)
						{
							// setting var_value
							btn_nd->slider->setValue(stoi(device_var_value.toStdString()));
							btn_nd->slider_val = stoi(device_var_value.toStdString());
						}

					}
				}
			}
		}
		cout << "------------- state render success --------------------" << endl;

		set_device_controller_api_request(GET_I2C_DATA);
		send_device_controller_api_request();
	}
}


void dashboard::render_i2c_data()
{
	if(device_controller_api_response_parse())
	{
		cout << "\n\\nI2C data : \n" << dashboard::get_device_controller_api_response() << endl << endl;
		// checking if device state is updated or not
		if(api_i2c_data == "" || api_i2c_data != dashboard::get_device_controller_api_response())
		{
			// set updated device state in temp variable
			api_i2c_data = dashboard::get_device_controller_api_response();

			// get_room_device_status api call
			set_device_controller_api_request(GET_ROOM_DEVICE_STATUS);
			send_device_controller_api_request();
		}
		// msleep(1000);
		set_device_controller_api_request(GET_I2C_DATA);
		send_device_controller_api_request();
	}
}


dashboard::~dashboard()
{
	delete ui;
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
