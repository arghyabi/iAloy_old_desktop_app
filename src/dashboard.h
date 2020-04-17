#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include<QString>

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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

#include "device_controller_api.h"

using namespace std;

namespace Ui {
class dashboard;
}

class dashboard : public QMainWindow, public device_controller_api
{

	Q_OBJECT

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
	QFrame *frame_d;

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

	~dashboard();

private slots:

private:
	Ui::dashboard *ui;

	QJsonArray get_json_array_from_response();
};

#endif // DASHBOARD_H
