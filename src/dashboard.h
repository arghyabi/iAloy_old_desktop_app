#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QString>
#include <math.h>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <QSignalMapper>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

#include "device_controller_api.h"
#include "main.h"
#include "i2c_data.h"

using namespace std;

namespace Ui {
class dashboard;
}

class dashboard : public QMainWindow, public device_controller_api
{

	Q_OBJECT
	QThread i2cWorkerThread;

public:
	/*
	v_layout_for_all_room => outer most layout for all room
	h_layout_for_room_and_spacer => contains the room laoyout and spacer
	v_layout_for_all_btn_node_and_room_label => contains room_label and multiple btn_node layout
	h_layout_for_all_btn_node =>contains multiple btn_node
	v_layout_for_btn_node => innermost layout for the btn and slider
	*/

	QVBoxLayout *v_layout_for_all_room, *v_layout_for_all_btn_node_and_room_label, *v_layout_for_btn_node;
	QHBoxLayout *h_layout_for_room_and_spacer, *h_layout_for_all_btn_node, *horizontalSpacer_3;
	QPushButton *btn1;
	QLabel *room_name_label;
	QSlider *slider1;
	QSpacerItem *spacerItem;

	QTimer *timer_for_datetime;
	QTimer *timer_for_i2c_data_read_from_web;
	QTimer *timer_for_i2c_data_read_from_mod;

	QSignalMapper *signalMapper;

	explicit dashboard(QWidget *parent = 0);
	void addBgImage();
	void init();
	void set_dashboard_network_interface(QNetworkAccessManager *QNAM, QNetworkRequest *QNR);
	bool dashboard_login_using_token();

	void update_dashboard_gui();
	void send_device_controller_api_request();
	bool device_controller_api_response_parse();

	// render methods
	void render_dashboard_login();
	void render_dashboard_room_btn();
	void render_dashboard_room_btn_state();
	void render_i2c_data();
	void clearLayout(QLayout* layout);
	~dashboard();

signals:
	void send_i2c_data_to_module_signal(int, int);
	void read_request_i2c_data_from_module_signal(int, int);

private slots:
	void update_time();
	void on_power_tool_button_clicked();
	void on_settings_tool_button_clicked();
	void on_wifi_tool_button_clicked();
	void on_keyboard_tool_button_clicked();
	void on_app_update_button_clicked();
	void on_logout_button_clicked();
	void get_i2c_web_status();
	void read_request_i2c_data_from_module();
	void read_i2c_data_from_module(int, int);
	void button_clicked_slot(QString);

private:
	Ui::dashboard *ui;

	QJsonArray get_json_array_from_response();
	int hex_to_int(string hex);
};

#endif // DASHBOARD_H
