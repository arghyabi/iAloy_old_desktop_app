#include<iostream>
#include<QMessageBox>
#include<QString>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connect_init.h"
#include "main.h"

using namespace std;

static int SetUpLineEdit_stat = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->status_label->setText("");
	ui->passsword_toggle->hide();
}

MainWindow::~MainWindow()
{
	delete ui;
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
			ui->status_label->setText("Checking account...");
			string email = ui->SetUpLineEdit->text().toStdString();
			MainWindow::set_email(email);
			int resp = stoi(MainWindow::check_email_pi_connection());
			cout << "RESP : " << resp << endl;

			if (resp == SUPER_USER_EMAIL)
			{
				cout << email << " is a super user account" << endl;
				ui->status_label->setText(QString::fromStdString(email)+" is a super user account");
				ui->SetUpLineEdit->setText("");
				ui->passsword_toggle->show();
				ui->SetUpLineEdit->setPlaceholderText("Enter password");
				ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
				ui->submit_button->setText("Login");
				SetUpLineEdit_stat = GET_INPUT_PASSWORD_MODE;
			}
			else if(resp == PERMANENT_USER_EMAIL)
			{
				cout << email << " is a permanent user account" << endl;
				ui->status_label->setText(QString::fromStdString(email)+" is a permanent user account");
				ui->SetUpLineEdit->setText("");
				ui->passsword_toggle->show();
				ui->SetUpLineEdit->setPlaceholderText("Enter password");
				ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
				ui->submit_button->setText("Login");
				SetUpLineEdit_stat = GET_INPUT_PASSWORD_MODE;
			}
			else
			{
				cout << email << " invalid user of this Pi" << endl;
				ui->SetUpLineEdit->setText("");
				ui->status_label->setText(QString::fromStdString(email)+" invalid user of this Pi");
				/*
					TO DO:
					PI_ID CHECKING
				*/
				// ui->SetUpLineEdit->setPlaceholderText("Enter Product Key...");
				// ui->SetUpLineEdit->setText("");
				// ui->SetUpLineEdit->setMaxLength(16);
				// ui->submit_button->setText("Procced");
				// SetUpLineEdit_stat = GET_INPUT_PROD_KEY_MODE;
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
			string prod_key = ui->SetUpLineEdit->text().toStdString();
			MainWindow::set_product_id(prod_key);
			int resp = stoi(MainWindow::check_product_id());
			cout << "RESP : " << resp << endl;

			if(resp == PRODUCT_KEY_AVAILABLE)
			{
				cout << "Product key is available... Its ok" << endl;
				ui->status_label->setText("Product key applied successfully.");
				ui->SetUpLineEdit->setText("");
				ui->SetUpLineEdit->setPlaceholderText("Enter password");
				ui->SetUpLineEdit->setEchoMode(QLineEdit::Password);
				ui->submit_button->setText("Login");
				SetUpLineEdit_stat = GET_INPUT_PASSWORD_MODE;
			}
			else if(resp == PRODUCT_KEY_NOT_AVAILABLE)
			{
				cout << "Product key not available... Change it" << endl;
				ui->SetUpLineEdit->setText("");
				ui->status_label->setText("Product key already in used.");
			}
			else
			{
				cout << "Invalid product key entered... Try again" << endl;
				ui->SetUpLineEdit->setText("");
				ui->status_label->setText("Invalid product key entered... Try again");
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
			ui->status_label->setText("Loading...");
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
				ui->status_label->setText("Wrong password... Try again");
			}
		}
		else
		{
			QMessageBox::warning(this, "Password is empty",
								 "Password can't be empty. Please put "
								 "your password to continue",QMessageBox::Ok);
		}
	}
}
