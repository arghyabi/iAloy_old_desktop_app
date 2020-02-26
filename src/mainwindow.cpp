#include<iostream>
#include<QMessageBox>
#include<QString>
#include<regex>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connect_init.h"
#include "main.h"

using namespace std;

static int SetUpLineEdit_stat = 0, user_type = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->status_label->setText("");
	ui->passsword_toggle->hide();
	ui->password_edit->setEchoMode(QLineEdit::Password);
	ui->c_password_edit->setEchoMode(QLineEdit::Password);
	hide_reg_form();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::status_label_set_text(string text, string color)
{
	ui->status_label->setText("<font color='"+QString::fromStdString(color)+"'>" \
		+QString::fromStdString(text)+"</font>");
}

bool MainWindow::emailCheck(string email)
{
	const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email,pattern);
}

void MainWindow::show_reg_form()
{
	MainWindow::set_email(ui->SetUpLineEdit->text().toStdString());
	if(MainWindow::email_reg_status())
	{
		user_type = EXISTING_USER;
		string user_details = MainWindow::get_user_details();
		int loc = user_details.find_first_of(",");
		string first_name = user_details.substr(0, loc);
		string last_name = user_details.substr(loc+1, user_details.find_first_of(",", loc+1));
		cout << "Email : " << MainWindow::get_email() << "F_name : " << first_name << "L_name : " << last_name << endl;

		status_label_set_text("<center>This Pi is not registered & don't have Super User. \
		<br>Fill the Super User details to register the Pi<center>", "#53ed11");
		ui->SetUpLineEdit->hide();
		ui->submit_button->hide();
		ui->f_name_edit->show();
		ui->f_name_edit->setText(QString::fromStdString(first_name));
		ui->l_name_edit->show();
		ui->l_name_edit->setText(QString::fromStdString(last_name));
		ui->email_id_edit->show();
		ui->email_id_edit->setText(QString::fromStdString(MainWindow::get_email()));
		ui->form_status_label->show();
		ui->form_back_btn->show();
		ui->form_reset_btn->show();
		ui->form_next_btn->show();
		ui->form_status_label->setText("You will be the Super User of this Pi... Please continue.");
		ui->email_id_edit->setText(QString::fromStdString(MainWindow::get_email()));
		ui->email_id_edit->setDisabled(1);
	}
	else
	{
		user_type = NEW_USER;
		status_label_set_text("<center>This Pi is not registered & don't have Super User. \
		<br>Fill the Super User details to register the Pi<center>", "#53ed11");
		ui->SetUpLineEdit->hide();
		ui->submit_button->hide();
		ui->f_name_edit->show();
		ui->f_name_edit->setText("");
		ui->l_name_edit->show();
		ui->f_name_edit->setText("");
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
	ui->status_label->setText("");
	ui->SetUpLineEdit->show();
	//ui->SetUpLineEdit->setEchoMode(QLineEdit::Text);
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
// string email_data, product_key_data, password_data;

void MainWindow::addBgImage()
{
	QString pic_add = "/usr/share/iAloy/back.jpg";
	QPixmap bkgnd(pic_add);
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

void MainWindow::on_submit_button_clicked()
{
	if (SetUpLineEdit_stat == GET_INPUT_EMAIL_MODE)
	{
		// Email id proccessing section
		if (ui->SetUpLineEdit->text() != "")
		{
			// check email is right formatted or not
			if(emailCheck(ui->SetUpLineEdit->text().toStdString()))
			{
				ui->status_label->setText("Checking account...");
				MainWindow::set_email(ui->SetUpLineEdit->text().toStdString());
				int resp = stoi(MainWindow::check_email_pi_connection());
				cout << "RESP : " << resp << endl;

				if (resp == SUPER_USER_EMAIL)
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
				else if(resp == PERMANENT_USER_EMAIL)
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
				else
				{
					MainWindow::set_pi_add();
					if(stoi(MainWindow::pi_reg_status()) == PI_REGISTERED)
					{
						// Pi registered.
						cout << MainWindow::get_email() << " invalid user of this Pi" << endl;
						ui->SetUpLineEdit->setText("");
						status_label_set_text(MainWindow::get_email()+" invalid user of this Pi", "red");
					}
					else
					{
						// Pi not registered yet... register to continue
						show_reg_form();
					}
				}
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
	else if (SetUpLineEdit_stat == GET_INPUT_PROD_KEY_MODE)
	{
		// Productkey processing section
		if (ui->SetUpLineEdit->text() != "")
		{
			MainWindow::set_product_id(ui->SetUpLineEdit->text().toStdString());
			if(MainWindow::get_product_id().length()==16)
			{
				status_label_set_text("Product key validating...", "black");
				// MainWindow::set_product_id(prod_key);
				int resp = stoi(MainWindow::check_product_id());
				cout << "RESP : " << resp << endl;

				if(resp == PRODUCT_KEY_AVAILABLE)
				{
					cout << "Product key is available... Its ok" << endl;
					// status_label_set_text("Product key applied successfully.", "green");

					if(MainWindow::send_otp())
					{
						ui->SetUpLineEdit->setText("");
						ui->SetUpLineEdit->setPlaceholderText("Enter OTP");
						// ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
						ui->SetUpLineEdit->setMaxLength(6);
						// ui->submit_button->setText("SUBMIT");
						SetUpLineEdit_stat = GET_INPUT_OTP_MODE;
					}
					else
					{
						ui->form_status_label->setText("Something wrong");
					}

				}
				else if(resp == PRODUCT_KEY_NOT_AVAILABLE)
				{
					cout << "Product key not available... Change it" << endl;
					ui->SetUpLineEdit->setText("");
					status_label_set_text("Product key already in used.", "red");
				}
				else
				{
					cout << "Invalid product key entered... Try again" << endl;
					ui->SetUpLineEdit->setText("");
					status_label_set_text("Invalid product key entered... Try again", "red");
				}
			}
			else
			{
				status_label_set_text("Product key must be 16 digit long", "black");
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
	else if (SetUpLineEdit_stat == GET_INPUT_PASSWORD_MODE)
	{
		if(ui->SetUpLineEdit->text() != "")
		{
			string password = ui->SetUpLineEdit->text().toStdString();
			MainWindow::set_password(password);
			status_label_set_text("Loading...","black");
			int resp = MainWindow::login();
			cout << resp << endl;
			if(resp)
			{
				dashboard_window_show(true);
				main_window_show(false);
			}
			else
			{
				ui->SetUpLineEdit->setText("");
				status_label_set_text("Wrong password... Try again", "red");
			}
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
		cout << "OTP : " << ui->SetUpLineEdit->text().toStdString() << endl;
		if(ui->SetUpLineEdit->text().toStdString() == MainWindow::otp)
		{
			if(user_type == NEW_USER)
			{
				cout << "OTP matched..." << endl;
				MainWindow::set_user_type(NEW_USER);
				MainWindow::set_f_name(ui->f_name_edit->text().toStdString());
				MainWindow::set_l_name(ui->l_name_edit->text().toStdString());
				MainWindow::set_password(ui->password_edit->text().toStdString());

				if(MainWindow::reg_new_pi())
				{
					cout << "Registered..." << endl;
					status_label_set_text("Registered successfully.", "green");
					ui->SetUpLineEdit->hide();
					ui->submit_button->setText("Dash Board");
				}
				else
				{
					cout << "Registration failed..." << endl;
					status_label_set_text("Registration failed.", "red");
				}
			}
			else
			{
				cout << "OTP matched..." << endl;
				MainWindow::set_user_type(EXISTING_USER);
				if(MainWindow::reg_new_pi())
				{
					cout << "Registered..." << endl;
					status_label_set_text("Registered successfully.", "green");
					ui->SetUpLineEdit->hide();
					ui->submit_button->setText("Dash Board");
				}
				else
				{
					cout << "Registration failed..." << endl;
					status_label_set_text("Registration failed.", "red");
				}
			}
			

		}
		else
		{
			cout << "OTP mismatched..." << endl;
			ui->SetUpLineEdit->setText("");
			status_label_set_text("OTP mismatched...", "red");
		}
		
	}
	else if(SetUpLineEdit_stat == GET_INPUT_PI_NAME_MODE)
	{
		if(ui->SetUpLineEdit->text() != "")
		{
			MainWindow::set_pi_name(ui->SetUpLineEdit->text().toStdString());
			// show product_key enter field
			status_label_set_text("Enter 16 digit product key to register", "black");
			ui->SetUpLineEdit->setPlaceholderText("Enter Product Key...");
			ui->SetUpLineEdit->setText("");
			ui->SetUpLineEdit->setMaxLength(16);
			ui->submit_button->setText("Procced");
			SetUpLineEdit_stat = GET_INPUT_PROD_KEY_MODE;
		}
	}
}

void MainWindow::on_form_back_btn_clicked()
{
	ui->SetUpLineEdit->setText(QString::fromStdString(MainWindow::get_email()));
	ui->SetUpLineEdit->setPlaceholderText("Email");
	ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
	ui->submit_button->setText("Go");
	hide_reg_form();
}

void MainWindow::on_form_next_btn_clicked()
{
	// show product_key enter field
	// status_label_set_text("Enter 16 digit product key to register", "black");
	// ui->SetUpLineEdit->setPlaceholderText("Enter Product Key...");
	// ui->SetUpLineEdit->setText("");
	// ui->SetUpLineEdit->setMaxLength(16);
	// ui->submit_button->setText("Procced");
	status_label_set_text("Set a Pi Name", "black");
	ui->SetUpLineEdit->setPlaceholderText("Enter Pi Name...");
	ui->SetUpLineEdit->setText("");
	ui->SetUpLineEdit->setMaxLength(40);
	ui->submit_button->setText("Procced");
	SetUpLineEdit_stat = GET_INPUT_PI_NAME_MODE;
	hide_reg_form();
}

void MainWindow::on_form_reset_btn_clicked()
{
	ui->f_name_edit->setText("");
	ui->l_name_edit->setText("");
	ui->password_edit->setText("");
	ui->c_password_edit->setText("");
	ui->form_status_label->setText("");
}

void MainWindow::on_passsword_toggle_stateChanged(int arg1)
{
	if(arg1 == 2)
		ui->SetUpLineEdit->setEchoMode(QLineEdit::Normal);
	else if (arg1 == 0)
		ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
}
