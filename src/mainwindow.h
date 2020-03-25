#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMainWindow>
#include <iostream>

#include "connect_init.h"
#include "ialoy_data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ialoy_web_api
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	void addBgImage();
	void status_label_set_text(string text, string color);
	void show_reg_form();
	void hide_reg_form();
	bool validateEmail(string email);

	void update_mainwindow_gui();
	bool api_response_parse();
	void send_api_request();

	void render_pi_name();
	void render_password_or_user_details_form();
	void render_after_login();
	void render_otp_send_form();
	void render_otp_verified_form();
	void render_data_in_show_reg_form();
	void render_product_key_validate_form();
	void render_registered_new_pi();

	~MainWindow();

private slots:
	void on_submit_button_clicked();
	void on_form_back_btn_clicked();
	void on_form_next_btn_clicked();
	void on_form_reset_btn_clicked();
	void on_passsword_toggle_stateChanged(int arg1);

private:
	QNetworkAccessManager *NetworkManager;
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
