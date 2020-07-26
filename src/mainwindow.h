#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMainWindow>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "connect_init.h"
#include "ialoy_data.h"
#include "update_manager_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ialoy_web_api
{
	Q_OBJECT

public:
	QTimer *timer_for_datetime_mm;
	QTimer *timer_for_get_pi_name;

	explicit MainWindow(QWidget *parent = nullptr);
	void status_label_set_text(string text, string color);
	void show_reg_form();
	void hide_reg_form();
	bool validateEmail(string email);

	void update_mainwindow_gui(int response_type);
	bool try_login_using_token();
	void send_api_request();

	void render_pi_name();
	void render_password_or_user_details_form();
	void render_after_login();
	void render_otp_send_form();
	void render_otp_verified_form();
	void render_data_in_show_reg_form();
	void render_product_key_validate_form();
	void render_registered_new_pi();
	void render_login_using_token();
	void mainwindow_reset_on_logout();
	void other_user_login_setup(QString);

	~MainWindow();

private slots:
	void update_time();
	void ip_address_update();
	void get_pi_name_with_timer_slot();
	void on_submit_button_clicked();
	void on_form_back_btn_clicked();
	void on_form_next_btn_clicked();
	void on_form_reset_btn_clicked();
	void on_passsword_toggle_stateChanged(int arg1);
	void on_keyboard_tool_button_clicked();
	void on_wifi_tool_button_clicked();
	void on_settings_tool_button_clicked();
	void on_app_update_button_clicked();
	void on_power_tool_button_clicked();
	void auto_update_available_check();
	void on_available_update_btn_clicked();
	void check_update_available_response_for_parent_slot(bool);

signals:
	void settings_window_show_signal(int);
	void check_update_available_signal();

private:
	Ui::MainWindow *ui;
	bool get_pi_name_timer_flag;
	int counter_for_ip_check;
};

#endif // MAINWINDOW_H
