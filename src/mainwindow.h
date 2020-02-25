#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<iostream>

#include "connect_init.h"

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
	GET_INPUT_OTP_MODE	
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ialoy_web_api
{
	Q_OBJECT

public:
	// std::string email;
	std::string prod_key;
	explicit MainWindow(QWidget *parent = nullptr);
	void addBgImage();
	void status_label_set_text(string text, string color);
	void show_reg_form();
	void hide_reg_form();
	bool emailCheck(string email);
	ialoy_web_api api;
	~MainWindow();

//private slots:

private slots:
	void on_submit_button_clicked();
	void on_form_back_btn_clicked();
	void on_form_next_btn_clicked();
	void on_form_reset_btn_clicked();
	void on_passsword_toggle_stateChanged(int arg1);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
