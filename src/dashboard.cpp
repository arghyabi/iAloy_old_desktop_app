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

// bool dashboard::dashboard_login_using_token()
// {
// 	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
// 	QFile file;
// 	file.setFileName(QString::fromStdString(this->get_user_credential_path()));

// 	if(!file.exists())
// 		return false;

// 	file.open(QIODevice::ReadOnly | QIODevice::Text);
// 	QString val = file.readAll();
// 	file.close();
// 	cout << "Raw file data : " << endl;

// 	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
// 	QJsonObject sett2 = d.object();
// 	QJsonValue value = sett2.value(QString("userCredential"));
// 	QJsonObject item = value.toObject();

// 	if(item["email"] != "" && item["token"] != "")
// 	{
// 		this->set_email(item["email"].toString().toStdString());
// 		this->set_token(item["token"].toString().toStdString());

// 		cout << "DB-Email : " << item["email"].toString().toStdString() << endl;
// 		cout << "DB-Token : " << item["token"].toString().toStdString() << endl;

// 		return true;
// 	}
// 	else
// 	{
// 		cout << "No saved user credential found... from DashBoard" << endl;
// 		return false;
// 	}
// }

// void dashboard::set_dashboard_network_interface(QNetworkAccessManager *QNAM, QNetworkRequest *QNR)
// {
// 	this->NetworkManager = QNAM;
// 	this->NetworkRequest = *QNR;
// }

void dashboard::send_device_controller_api_request()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(this->get_device_controller_api_error_msg() == "")
		DBNetworkManager->get(NetworkRequest);
	else
		cout << "Error massage : " << this->get_device_controller_api_error_msg() << endl;
}

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

		default:
			break;
	}
}


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


void dashboard::render_dashboard_room_btn()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	if(this->device_controller_api_response_parse())
	{

		string room_device_data = this->get_device_controller_api_response();
		cout << "\n\nRoom_device data : " << room_device_data << endl << endl;

		QPushButton *btn1;
		QVBoxLayout *verticalLayout_4, *verticalLayout01, *verticalLayout1;
		QHBoxLayout *horizontalLayout01, *horizontalLayout1, *horizontalSpacer_3;
		QLabel *room_name_label;
		QSlider *slider1;
		QSpacerItem *spacerItem;

		// creating main_room_container
		verticalLayout_4 = new QVBoxLayout();
		verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));


		QString room_device_info = QString::fromStdString(room_device_data);

		QJsonDocument jsonDocument = QJsonDocument::fromJson(room_device_info.toUtf8());
		QJsonObject jsonObject = jsonDocument.object();

		QJsonValue roomArrayValue = jsonObject.value("room_device_details");
		QJsonArray roomArray = roomArrayValue.toArray();

		foreach (const QJsonValue &room, roomArray)
		{
			horizontalLayout01 = new QHBoxLayout();
			QString sub_room_container_object_name = "sub_room_container_"+room.toObject().value("room_id").toString();
			horizontalLayout01->setObjectName(sub_room_container_object_name);

			// creating room
			verticalLayout01 = new QVBoxLayout();
			QString room_object_name = "room_"+room.toObject().value("room_id").toString();
			verticalLayout01->setObjectName(room_object_name);

			// room_name
			room_name_label = new QLabel();
			QString room_name = room.toObject().value("room_name").toString();
			room_name_label->setObjectName(room_name);
			room_name_label->setText(room_name);

			// adding room_name to room
			verticalLayout01->addWidget(room_name_label);

			// creating device_container
			horizontalLayout1 = new QHBoxLayout();
			QString device_container_name = "device_container_"+room.toObject().value("room_id").toString();
			horizontalLayout1->setObjectName(device_container_name);

			QJsonArray dev_array = room.toObject().value("dev_list").toArray();

			foreach (const QJsonValue &device, dev_array)
			{
				QString device_name = device.toObject().value("d_name").toString();
				QString device_id = device.toObject().value("dev_id").toString();
				QString device_is_var = device.toObject().value("is_var").toString();

				// creating dynamic devices

				// creating device_layout
				verticalLayout1 = new QVBoxLayout();
				QString device_layout_obj_name = "device_layout_"+device_id;
				verticalLayout1->setObjectName(device_layout_obj_name);

				// creating device_btn
				btn1 = new QPushButton();
				QString device_button_name = "device_button_"+device_id;
				btn1->setObjectName(device_button_name);
				QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
				sizePolicy.setHorizontalStretch(0);
				sizePolicy.setVerticalStretch(0);
				sizePolicy.setHeightForWidth(btn1->sizePolicy().hasHeightForWidth());
				btn1->setSizePolicy(sizePolicy);
				btn1->setMinimumSize(QSize(0, 0));
				btn1->setText(device_name);

				// add device_btn to device_layout
				verticalLayout1->addWidget(btn1);

				// creating device_slider
				slider1 = new QSlider();
				QString device_slider_name = "device_slider_"+device_id;
				slider1->setObjectName(device_slider_name);
				sizePolicy.setHeightForWidth(slider1->sizePolicy().hasHeightForWidth());
				slider1->setSizePolicy(sizePolicy);
				slider1->setOrientation(Qt::Horizontal);

				// add device_slider to device_layout
				verticalLayout1->addWidget(slider1);

				// add device_layout to device_container
				horizontalLayout1->addLayout(verticalLayout1);

			}

			// adding device_container to room
			verticalLayout01->addLayout(horizontalLayout1);

			// adding room to sub_room_container
			horizontalLayout01->addLayout(verticalLayout01);

			// create spacer
			spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

			// add spacer to sub_room_container
			horizontalLayout01->addItem(spacerItem);

			// add sub_room_container to main_room_container
			ui->verticalLayout_4->addLayout(horizontalLayout01);

			cout << "------------- render success --------------------" << endl;
		}

	}
	// get api_response
	// string room_device_data = "\
{\
	\"room\": [{ \
	\"room_name\": \"room_no_1\",\"room_id\": \"120\", \
	\"dev_list\": [ \
	{\"dev_id\":\"405\",\"d_name\": \"led_5\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"404\",\"d_name\": \"led_4\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"403\",\"d_name\": \"led_3\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"402\",\"d_name\": \"led_2\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"401\",\"d_name\": \"led_1\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"417\",\"d_name\": \"led_8\", \"is_var\":\"1\"}] \
},{\"room_name\":\"room_no_2\",\"room_id\":\"121\", \
	\"dev_list\":[ \
	{\"dev_id\":\"426\",\"d_name\": \"ND_8\", \"is_var\": \"0\"}, \
	{\"dev_id\":\"425\",\"d_name\": \"ND_7\", \"is_var\": \"0\"}, \
	{\"dev_id\":\"402\",\"d_name\": \"led_2\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"401\",\"d_name\": \"led_1\", \"is_var\":\"0\"}, \
	{\"dev_id\":\"417\",\"d_name\": \"led_8\", \"is_var\":\"1\"} \
]}] \
}";

	// cout << "\n\nRoom_device_data : " << room_device_data << endl;

	// qt variables

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
