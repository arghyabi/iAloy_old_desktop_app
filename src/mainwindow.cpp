#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<QMessageBox>

#include "connect_init.h"

using namespace std;

static int SetUpLineEdit_stat = 0;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->SuEmailLabel->hide();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::addBgImange()
{
	QString pic_add = "/usr/share/iAloy/back.jpg";
	QPixmap bkgnd(pic_add);
	bkgnd = bkgnd.scaled(this->width(),this->height());
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

void MainWindow::on_SuEmailSubmit_clicked()
{
	if (SetUpLineEdit_stat == 0){
		if(ui->SetUpLineEdit->text() != ""){
			QString email = ui->SetUpLineEdit->text();
			string email_data = email.toStdString();
			ui->SuEmailLabel->show();
			string resp = call_web_api(SetUpLineEdit_stat, email_data, "456");
			cout << resp << endl;
			ui->SuEmailLabel->setText(QString::fromStdString(resp));
			ui->SetUpLineEdit->setText("");
			ui->SetUpLineEdit->setPlaceholderText("Enter Product Key...");
			ui->SetUpLineEdit->setMaxLength(12);
			SetUpLineEdit_stat = 1;
		}else{
			QMessageBox::warning(this, "Email is empty",
								 "Email can't be empty. Please put "
								 "your email to continue",QMessageBox::Ok);
		}
	}else if(SetUpLineEdit_stat == 1){
		if(ui->SetUpLineEdit->text() != ""){
			QString product_key = ui->SetUpLineEdit->text();
			ui->SetUpLineEdit->hide();
			ui->SuEmailLabel->setText(ui->SuEmailLabel->text()+"\nCheck your email & procced to setup...");
			ui->SuEmailSubmit->setText("Procced");
			SetUpLineEdit_stat = 2;
		}else{
			QMessageBox::warning(this, "Product Id is empty",
								 "Product Id can't be empty. Please put "
								 "your email to continue",QMessageBox::Ok);
		}
	}else if(SetUpLineEdit_stat == 2){
		// Check web_api for setup complete...
		ui->SuEmailSubmit->setText("Procced");
	}

}
