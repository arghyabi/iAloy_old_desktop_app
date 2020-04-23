#include<iostream>
#include<QMessageBox>
#include<QString>
#include<regex>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connect_init.h"

using namespace std;

static int SetUpLineEdit_stat = 0, user_type = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->setupUi(this);

	ui->ialoy_logo_label->setText("<b><font size=8 color='#1e93b6'>i</font><font size=8 color='#555'>Aloy</font></b>");
	ui->ialoy_tag_line_label->setText("<font size=4 color='#415c76'><b>Smart Home for Smart Future</b></font>");
	ui->pi_name_label->setText(QString::fromStdString("<center><font size=3>Welcome to</font><b><br/><font size=5>"+this->get_pi_name()+"</font></b></center>"));
	// ui->pi_name_label->setText(QString::fromStdString(MainWindow::get_pi_name()));
	QTimer *timer_for_datatime = new QTimer(this);
	connect( timer_for_datatime, SIGNAL(timeout()), this, SLOT(update_time()) );
	timer_for_datatime->start(1000);

	ui->settings_tool_button->setIcon(QIcon(QString::fromStdString(MainWindow::get_settings_icon_path())));
	ui->wifi_tool_button->setIcon(QIcon(QString::fromStdString(MainWindow::get_wifi_icon_path())));
	ui->keyboard_tool_button->setIcon(QIcon(QString::fromStdString(MainWindow::get_keyboad_icon_path())));
	ui->app_update_button->setIcon(QIcon(QString::fromStdString(MainWindow::get_update_icon_path())));
	ui->power_tool_button->setIcon(QIcon(QString::fromStdString(MainWindow::get_power_icon_path())));

	NetworkManager = new QNetworkAccessManager();
	QObject::connect(NetworkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
			set_api_response("");
			if (reply->error()) {
				cout << "Error : " << reply->errorString().toStdString() << endl;
				return;
			}
			QString response = reply->readAll();
			cout << "MainWindow response : " << response.toStdString() << endl;
			set_api_response(response.toStdString());
			update_mainwindow_gui();
		}
	);

	ui->passsword_toggle->hide();
	ui->password_edit->setEchoMode(QLineEdit::Password);
	ui->c_password_edit->setEchoMode(QLineEdit::Password);
	hide_reg_form();
	ui->SetUpLineEdit->setDisabled(1);
	ui->submit_button->setDisabled(1);
	ui->pi_name_label->setText(QString::fromStdString("Connecting..."));
	ui->status_label->setText("");

	status_label_set_text("Checking saved login...", "black");
	if(!MainWindow::saved_credential_manager())
	{
		MainWindow::set_api_request(GET_PI_NAME);
	}
	else
	{
		MainWindow::set_api_request(LOGIN_USING_TOKEN);
	}

	send_api_request();
}

MainWindow::~MainWindow()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
	delete NetworkManager;
}

void MainWindow::update_time()
{
	// QDateTime dateTime = dateTime.currentDateTime();
	// QString dateTimeString = dateTime.toString("dd-MMMM-yyyy hh:mm:ss ap");
	// ui->time_label->setText(dateTimeString);
	QDateTime dateTime = dateTime.currentDateTime();
	QString timeString = dateTime.toString("hh:mm:ss ap");
	QString dateString = dateTime.toString("dd-MMMM-yyyy");
	ui->time_label->setText("<font size=4><b>"+timeString+"</b></font><br/>  "+dateString);
}

void MainWindow::mainwindow_reset_on_logout()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	SetUpLineEdit_stat = GET_INPUT_EMAIL_MODE;
	MainWindow::set_api_request(GET_PI_NAME);
	ui->SetUpLineEdit->setText("");
	ui->submit_button->setText("Go");
	ui->SetUpLineEdit->setPlaceholderText("Email");
	ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
	send_api_request();
}

void MainWindow::status_label_set_text(string text, string color)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->status_label->setText("<font color='"+QString::fromStdString(color)+"'>" \
		+QString::fromStdString(text)+"</font>");
}

bool MainWindow::validateEmail(string email)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	return regex_match(email,pattern);
}

void MainWindow::send_api_request()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(MainWindow::get_api_error_msg() == "")
		NetworkManager->get(NetworkRequest);
	else
	{
		cout << MainWindow::get_api_error_msg() << endl;
	}
}

void MainWindow::update_mainwindow_gui()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "flag:" << api_request << endl;
	switch(api_request)
	{
		case CHECK_PI_STATUS:
			break;

		case GET_PI_NAME:
			render_pi_name();
			break;

		case CHECK_EMAIL_STATUS:
			break;
		case CHECK_EMAIL_CONNECTED_PI:
			render_password_or_user_details_form();
			break;

		case GET_USER_DETAILS:
			render_data_in_show_reg_form();
			break;

		case CHECK_PRODUCT_KEY_STATUS:
			render_product_key_validate_form();
			break;

		case SEND_OTP:
			render_otp_send_form();
			break;

		case VERIFY_OTP:
			render_otp_verified_form();
			break;

		case REGISTER_NEW_PI:
			render_registered_new_pi();
			break;

		case LOGIN:
			render_after_login();
			break;

		case LOGIN_USING_TOKEN:
			render_login_using_token();
			break;

		default:
			break;
	}
}

void MainWindow::render_login_using_token()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		switch(user_login_status_flag)
		{
			case LOGIN_SUCCESS:
				cout << "Login success... from render()" << endl;
				dashboard_window_show(true);
				main_window_show(false);
				break;

			case LOGIN_FAIL:
				cout << "Login failed... from render()" << endl;
				status_label_set_text("No proper saved credential found... Please login", "black");
				MainWindow::set_api_request(GET_PI_NAME);
				send_api_request();
				break;
		}
	}
}

bool MainWindow::api_response_parse()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	string api_res = get_api_response();
	if(api_res.substr(0, 1) != "1")
		return false;

	api_res = api_res.substr(1, api_res.length()-1);

	switch(api_request)
	{
		case CHECK_PI_STATUS:
			break;

		case GET_PI_NAME:
			{
				if(stoi(api_res.substr(0,1)) == PI_REGISTERED)
				{
					api_res = api_res.substr(1, api_res.length()-1);
					pi_reg_status_flag = PI_REGISTERED;
					set_pi_name(api_res);
				}
				else if(stoi(api_res.substr(0,1)) == PI_NOT_REGISTERED)
				{
					pi_reg_status_flag = PI_NOT_REGISTERED;
					set_pi_name("Pi not registered");
				}
				break;
			}

		case CHECK_EMAIL_STATUS:
			break;

		case CHECK_EMAIL_CONNECTED_PI:
			{
				cout << "API_RESPONSE : " << api_res << endl;
				switch(stoi(api_res))
				{
					case SUPER_USER_REGISTERED_PI:
						reg_user_type_flag = EXISTING_USER;
						email_connected_pi_status_flag = SUPER_USER_REGISTERED_PI;
						break;
					case PERMANENT_USER_REGISTERED_PI:
						reg_user_type_flag = EXISTING_USER;
						email_connected_pi_status_flag = PERMANENT_USER_REGISTERED_PI;
						break;
					case INVALID_USER_REGISTERED_PI:
						reg_user_type_flag = NEW_USER;
						email_connected_pi_status_flag = INVALID_USER_REGISTERED_PI;
						break;
					case PERMANENT_USER_UNREGISTERED_PI:
						reg_user_type_flag = EXISTING_USER;
						email_connected_pi_status_flag = PERMANENT_USER_UNREGISTERED_PI;
						break;
					case INVALID_USER_UNREGISTERED_PI:
						reg_user_type_flag = NEW_USER;
						email_connected_pi_status_flag = INVALID_USER_UNREGISTERED_PI;
						break;
				}
				break;
			}
		case GET_USER_DETAILS:
			{
				int loc = api_res.find_first_of(",");
				string first_name = api_res.substr(0, loc);
				string last_name = api_res.substr(loc+1, api_res.find_first_of(",", loc+1));
				cout << "Email : " << MainWindow::get_email() << "F_name : " << first_name << "L_name : " << last_name << endl;
				MainWindow::set_first_name(first_name);
				MainWindow::set_last_name(last_name);
				break;
			}
		case CHECK_PRODUCT_KEY_STATUS:
			{
				switch(stoi(api_res))
				{
					case PRODUCT_KEY_AVAILABLE:
						product_Key_status_flag = PRODUCT_KEY_AVAILABLE;
						break;

					case PRODUCT_KEY_NOT_AVAILABLE:
						product_Key_status_flag = PRODUCT_KEY_NOT_AVAILABLE;
						break;

					case PRODUCT_KEY_INVALID:
						product_Key_status_flag = PRODUCT_KEY_INVALID;
						break;
				}
				break;
			}

		case SEND_OTP:
			{
				switch(stoi(api_res))
				{
					case OTP_SEND_SUCCESS:
						otp_send_status_flag = OTP_SEND_SUCCESS;
						break;

					case OTP_SEND_FAIL:
						otp_send_status_flag = OTP_SEND_FAIL;
						break;
				}
				break;
			}

		case VERIFY_OTP:
			{
				if(stoi(api_res) == OTP_VERIFIED)
					otp_verification_status_flag = OTP_VERIFIED;
				else if(stoi(api_res) == OTP_NOT_VERIFIED)
					otp_verification_status_flag = OTP_NOT_VERIFIED;
				break;
			}

		case REGISTER_NEW_PI:
			{
				cout << "API_REG_RESP : " << api_res << endl;
				switch(stoi(api_res))
				{
					case PI_REGISTERED:
						pi_reg_status_flag = PI_REGISTERED;
						break;

					case PI_NOT_REGISTERED:
						pi_reg_status_flag = PI_NOT_REGISTERED;
						break;
				}
				break;
			}

		case LOGIN:
			{
				string login_stat = api_res.substr(0,1);
				if(stoi(login_stat) == LOGIN_SUCCESS)
				{
					string token = api_res.substr(1, api_res.length()-1);
					cout << "Token : " << token << endl;
					if(token != "")
					{
						cout << "Login success..." << endl;
						MainWindow::set_token(token);
						MainWindow::store_user_credential();
						user_login_status_flag = LOGIN_SUCCESS;
					}
					else
					{
						cout << "Login success... But token is empty" << endl;
						MainWindow::set_token("");
						user_login_status_flag = LOGIN_TOKEN_ERROR;
					}
				}
				else
				{
					cout << "Login failed..." << endl;
					MainWindow::set_token("");
					user_login_status_flag = LOGIN_FAIL;
				}
			}
			break;

		case LOGIN_USING_TOKEN:
			{
				if(stoi(api_res) == LOGIN_SUCCESS)
				{
					cout << "Login success... from api_response_parse()" << endl;
					user_login_status_flag = LOGIN_SUCCESS;
				}
				else
				{
					cout << "Login failed... from api_response_parse()" << endl;
					user_login_status_flag = LOGIN_FAIL;
				}
			}
			break;

		default:
			break;
	}
	return true;
}

void MainWindow::render_registered_new_pi()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		switch(pi_reg_status_flag)
		{
			case PI_REGISTERED:
				cout << "Pi registered success..." << endl;
				status_label_set_text("Pi register sucess...", "green");
				dashboard_window_show(true);
				main_window_show(false);
				break;

			case PI_NOT_REGISTERED:
				cout << "Pi registered error..." << endl;
				status_label_set_text("Pi register error...", "red");
				break;
		}
	}
}

void MainWindow::render_product_key_validate_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		// render_product_key_validate_form
		switch(product_Key_status_flag)
		{
			case PRODUCT_KEY_AVAILABLE:
				set_api_request(SEND_OTP);
				send_api_request();
				break;

			case PRODUCT_KEY_NOT_AVAILABLE:
				ui->SetUpLineEdit->setText("");
				status_label_set_text("The product key you entered is not available... Please use different one", "black");
				break;

			case PRODUCT_KEY_INVALID:
				ui->SetUpLineEdit->setText("");
				status_label_set_text("The product key you entered is not valid... Please use correct one", "black");
				break;
		}
	}
	else
	{
		cout << "Error in api_response_parse()... from render_product_key_validate_form()" << endl;
	}
}

void MainWindow::render_data_in_show_reg_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		ui->f_name_edit->setText(QString::fromStdString(this->get_first_name()));
		ui->l_name_edit->setText(QString::fromStdString(this->get_last_name()));
		ui->email_id_edit->setText(QString::fromStdString(MainWindow::get_email()));
	}
	else
	{
		cout << "Error in api_response_parse()... from render_data_in_show_reg_form()" << endl;
	}
}

void MainWindow::render_otp_send_form()
{
	// OTP send form render
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		switch (otp_send_status_flag)
		{
		case OTP_SEND_SUCCESS:
			ui->SetUpLineEdit->setText("");
			ui->SetUpLineEdit->setMaxLength(6);
			ui->SetUpLineEdit->setPlaceholderText("Enter OTP...");
			status_label_set_text("Check your email and enter the OTP...", "black");
			SetUpLineEdit_stat = GET_INPUT_OTP_MODE;
			break;

		case OTP_SEND_FAIL:
			// OTP send failed area.
			break;

		default:
			break;
		}
	}
}

void MainWindow::render_otp_verified_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		if(otp_verification_status_flag == OTP_VERIFIED)
		{
			status_label_set_text("OTP verified... Registering new Pi...", "green");
			set_api_request(REGISTER_NEW_PI);
			send_api_request();
		}
		else if(otp_verification_status_flag == OTP_NOT_VERIFIED)
		{
			// OTP verificatoin failed form render.
			ui->SetUpLineEdit->setText("");
			status_label_set_text("OTP not matched... Please enter correct OTP", "red");
		}
	}
	else
	{
		cout << "API response error." << endl;
	}
}

void MainWindow::render_pi_name()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		if(pi_reg_status_flag == PI_NOT_REGISTERED)
		{
			cout << this->get_pi_name() << endl;
			status_label_set_text("", "black");
			ui->pi_name_label->setText(QString::fromStdString("Pi not registered"));
			ui->SetUpLineEdit->setPlaceholderText("Enter email to register");
		}
		else
		{
			cout << "Pi_name : " << this->get_pi_name() << endl;
			status_label_set_text("No proper saved credential found... Please login", "black");
			ui->pi_name_label->setText(QString::fromStdString("<center><font size=3>Welcome to</font><b><br/><font size=5>"+this->get_pi_name()+"</font></b></center>"));
		}
		ui->SetUpLineEdit->setDisabled(0);
		ui->submit_button->setDisabled(0);
	}
	else
	{
		cout << "API response error." << endl;
	}
}

void MainWindow::render_password_or_user_details_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		if (email_connected_pi_status_flag == SUPER_USER_REGISTERED_PI)
		{
			cout << MainWindow::get_email() << " is a super user account" << endl;
			status_label_set_text(MainWindow::get_email()+" is a super user account", "green");
			ui->SetUpLineEdit->setText("");
			ui->passsword_toggle->show();
			ui->SetUpLineEdit->setPlaceholderText("Enter password");
			ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
			ui->submit_button->setText("Login");
			SetUpLineEdit_stat = GET_INPUT_PASSWORD_MODE;
		}
		else if(email_connected_pi_status_flag == PERMANENT_USER_REGISTERED_PI)
		{
			cout << MainWindow::get_email() << " is a permanent user account" << endl;
			status_label_set_text(MainWindow::get_email()+" is a permanent user account", "green");
			ui->SetUpLineEdit->setText("");
			ui->passsword_toggle->show();
			ui->SetUpLineEdit->setPlaceholderText("Enter password");
			ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
			ui->submit_button->setText("Login");
			SetUpLineEdit_stat = GET_INPUT_PASSWORD_MODE;
		}
		else if(email_connected_pi_status_flag == INVALID_USER_REGISTERED_PI)
		{
			// user_details form render.
			status_label_set_text("Invalid user of this Pi... Please enter a registered email", "red");
			ui->SetUpLineEdit->setText("");
		}
		else if(email_connected_pi_status_flag == PERMANENT_USER_UNREGISTERED_PI || email_connected_pi_status_flag == INVALID_USER_UNREGISTERED_PI)
		{
			// user_details form render.
			show_reg_form();
		}
	}
	else
	{
		cout << "API response error." << endl;
	}
}

void MainWindow::render_after_login()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(api_response_parse())
	{
		if(user_login_status_flag == LOGIN_SUCCESS)
		{
			dashboard_window_show(true);
			main_window_show(false);
		}
		else if(user_login_status_flag == LOGIN_TOKEN_ERROR)
		{
			ui->SetUpLineEdit->setText("");
			status_label_set_text("Something went wrong... Please try again", "red");
		}
		else
		{
			ui->SetUpLineEdit->setText("");
			status_label_set_text("Wrong password... Try again", "red");
		}
	}
	else
	{
		ui->SetUpLineEdit->setText("");
		status_label_set_text("Something went wrong... Please try again", "red");
	}
}

void MainWindow::show_reg_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	MainWindow::set_email(ui->SetUpLineEdit->text().toStdString());
	if(reg_user_type_flag == EXISTING_USER)
	{
		status_label_set_text("<center>This Pi is not registered \
		<br>You will be the Super User of this Pi... \
		<br>Please fillup the details & continue.<center>", "#184d80");
		ui->SetUpLineEdit->hide();
		ui->submit_button->hide();
		ui->f_name_edit->show();
		ui->l_name_edit->show();
		ui->email_id_edit->show();
		ui->form_status_label->show();
		ui->form_back_btn->show();
		ui->form_reset_btn->show();
		ui->form_next_btn->show();
		ui->form_status_label->setText("");
		ui->email_id_edit->setDisabled(1);

		set_api_request(GET_USER_DETAILS);
		send_api_request();
	}
	else
	{
		status_label_set_text("<center>This Pi is not registered \
		<br>You will be the Super User of this Pi... \
		<br>Please fillup the details & continue.<center>", "#184d80");
		ui->SetUpLineEdit->hide();
		ui->submit_button->hide();
		ui->f_name_edit->show();
		ui->f_name_edit->setText("");
		ui->l_name_edit->show();
		ui->l_name_edit->setText("");
		ui->email_id_edit->show();
		ui->password_edit->show();
		ui->c_password_edit->show();
		ui->form_status_label->show();
		ui->form_back_btn->show();
		ui->form_reset_btn->show();
		ui->form_next_btn->show();
		ui->form_status_label->setText("");
		ui->email_id_edit->setText(QString::fromStdString(MainWindow::get_email()));
		ui->email_id_edit->setDisabled(1);
	}
}

void MainWindow::hide_reg_form()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->status_label->setText("");
	ui->SetUpLineEdit->show();
	ui->submit_button->show();
	ui->f_name_edit->hide();
	ui->l_name_edit->hide();
	ui->email_id_edit->hide();
	ui->password_edit->hide();
	ui->c_password_edit->hide();
	ui->form_status_label->hide();
	ui->form_back_btn->hide();
	ui->form_reset_btn->hide();
	ui->form_next_btn->hide();
}

void MainWindow::addBgImage()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QPixmap bkgnd(QString::fromStdString(MainWindow::get_mainwindow_bg_file_path()));
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

void MainWindow::on_submit_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if (SetUpLineEdit_stat == GET_INPUT_EMAIL_MODE)
	{
		// Email id proccessing section
		if (ui->SetUpLineEdit->text() != "")
		{
			ui->status_label->setText("Checking account...");
			// check email is right formatted or not
			if(validateEmail(ui->SetUpLineEdit->text().toStdString()))
			{
				MainWindow::set_email(ui->SetUpLineEdit->text().toStdString());
				set_api_request(CHECK_EMAIL_CONNECTED_PI);
				send_api_request();
			}
			else
			{
				ui->SetUpLineEdit->setText("");
				status_label_set_text("This is not an valid email... ", "red");
			}
		}
		else
		{
			QMessageBox::warning(this, "Email is empty",
								 "Email can't be empty. Please put "
								 "your email to continue",QMessageBox::Ok);
		}
	}
	else if (SetUpLineEdit_stat == GET_INPUT_PASSWORD_MODE)
	{
		if(ui->SetUpLineEdit->text() != "")
		{
			string password = ui->SetUpLineEdit->text().toStdString();
			MainWindow::set_password(password);
			status_label_set_text("Logging in...","black");
			set_api_request(LOGIN);
			send_api_request();
		}
		else
		{
			QMessageBox::warning(this, "Password is empty",
								 "Password can't be empty. Please put "
								 "your password to continue",QMessageBox::Ok);
		}
	}
	else if(SetUpLineEdit_stat == GET_INPUT_OTP_MODE)
	{

		if(ui->SetUpLineEdit->text().toStdString() != "" && ui->SetUpLineEdit->text().toStdString().length() == 6)
		{
			status_label_set_text("Checking OTP...", "black");
			MainWindow::set_otp(ui->SetUpLineEdit->text().toStdString());
			set_api_request(VERIFY_OTP);
			send_api_request();
		}
		else
		{
			ui->SetUpLineEdit->setText("");
			status_label_set_text("Your OTP should be 6 digit long... Please enter again.", "red");
		}
	}
	else if(SetUpLineEdit_stat == GET_INPUT_PI_NAME_MODE)
	{
		if(ui->SetUpLineEdit->text() != "")
		{
			MainWindow::set_pi_name(ui->SetUpLineEdit->text().toStdString());
			cout << "Pi_name : " << MainWindow::get_pi_name() << endl;
			// show product_key enter field
			status_label_set_text("Enter 16 digit product key to register", "black");
			ui->SetUpLineEdit->setPlaceholderText("Enter Product Key...");
			ui->SetUpLineEdit->setText("");
			ui->SetUpLineEdit->setMaxLength(16);
			ui->submit_button->setText("Procced");
			SetUpLineEdit_stat = GET_INPUT_PROD_KEY_MODE;
		}
	}
	else if (SetUpLineEdit_stat == GET_INPUT_PROD_KEY_MODE)
	{
		// Product key processing section
		if (ui->SetUpLineEdit->text() != "")
		{
			if(ui->SetUpLineEdit->text().toStdString().length() == 16)
			{
				MainWindow::set_product_id(ui->SetUpLineEdit->text().toStdString());
				status_label_set_text("Product key validating...", "black");
				set_api_request(CHECK_PRODUCT_KEY_STATUS);
				send_api_request();
			}
			else
			{
				status_label_set_text("Product key must be 16 digit long", "red");
				ui->SetUpLineEdit->setText("");
			}
		}
		else
		{
			QMessageBox::warning(this, "Product Id is empty",
								 "Product Id can't be empty. Please put "
								 "your Product Id to continue",QMessageBox::Ok);
		}
	}
}

void MainWindow::on_form_back_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->SetUpLineEdit->setText(QString::fromStdString(MainWindow::get_email()));
	ui->SetUpLineEdit->setPlaceholderText("Email");
	ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
	ui->submit_button->setText("Go");
	hide_reg_form();
}

void MainWindow::on_form_next_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	// set_data in api
	if(reg_user_type_flag == NEW_USER)
	{
		MainWindow::set_first_name(ui->f_name_edit->text().toStdString());
		MainWindow::set_last_name(ui->l_name_edit->text().toStdString());
		MainWindow::set_email(ui->email_id_edit->text().toStdString());
		if(ui->password_edit->text() == ui->c_password_edit->text())
			MainWindow::set_password(ui->password_edit->text().toStdString());
		else
		{
			ui->password_edit->setText("");
			ui->c_password_edit->setText("");
			QMessageBox::warning(this, "Password mismatched",
								"Password and confirm password not matched ", QMessageBox::Ok);
			return;
		}
		cout << "Email : " << MainWindow::get_email() << "\nFirst name : " << MainWindow::get_first_name() << "\nLast name : " << MainWindow::get_last_name() << "\nPassword : " << MainWindow::get_password() << endl;
	}
	else
	{
		cout << "Email : " << MainWindow::get_email() << "\nFirst name : " << MainWindow::get_first_name() << "\nLast name : " << MainWindow::get_last_name() << endl;
	}


	status_label_set_text("Set a Pi Name", "black");
	ui->SetUpLineEdit->setPlaceholderText("Enter Pi Name...");
	ui->SetUpLineEdit->setText("");
	ui->SetUpLineEdit->setMaxLength(40);
	ui->submit_button->setText("Next");
	SetUpLineEdit_stat = GET_INPUT_PI_NAME_MODE;
	hide_reg_form();
}

void MainWindow::on_form_reset_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->f_name_edit->setText("");
	ui->l_name_edit->setText("");
	ui->password_edit->setText("");
	ui->c_password_edit->setText("");
	ui->form_status_label->setText("");
}

void MainWindow::on_passsword_toggle_stateChanged(int arg1)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(arg1 == 2)
		ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
	else if (arg1 == 0)
		ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
}

void MainWindow::on_power_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
#ifdef ARC_TYPE
	system("reboot");
#else
	this->close();
#endif
}

void MainWindow::on_keyboard_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

void MainWindow::on_wifi_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
}

void MainWindow::on_settings_tool_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	settings_window_show(true);
}

void MainWindow::on_app_update_button_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	update_manager_window_show(true);
}
