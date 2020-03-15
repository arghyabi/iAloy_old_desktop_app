#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMainWindow>
#include <iostream>

#include "connect_init.h"
#include "ialoy_data.h"

enum email_status
{
	SUPER_USER_EMAIL = 11,
	PERMANENT_USER_EMAIL = 10,
	INVALID_USER_EMAIL = 0
};

enum setUpLineEditStat
{
	GET_INPUT_EMAIL_MODE = 0,
	GET_INPUT_PROD_KEY_MODE,
	GET_INPUT_PASSWORD_MODE,
	GET_INPUT_OTP_MODE,
	GET_INPUT_PI_NAME_MODE
};

enum prodKey_status
{
	PRODUCT_KEY_AVAILABLE = 0,
	PRODUCT_KEY_NOT_AVAILABLE,
	PRODUCT_KEY_INVALID
};

enum pi_reg_status
{
	PI_UNREGISTERED = 0,
	PI_REGISTERED
};

enum user_type
{
	EXISTING_USER = 0,
	NEW_USER
};

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
	bool emailCheck(string email);

	void update_mainwindow_gui();
	bool api_response_parse();
	void render_pi_name();

	~MainWindow();

//private slots:

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
