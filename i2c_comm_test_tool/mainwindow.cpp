#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include <iostream>
#include <unistd.h>
#ifdef ARC_TYPE
#include <wiringPiI2C.h>
#endif
#define MAX_DATA 255

using namespace std;

int data_array[MAX_DATA];
int index_val = 0;


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->data_lineEdit->setValidator( new QIntValidator(0, 255, this));
	ui->time_lineEdit->setValidator( new QIntValidator(0, 1000000000, this));

	ui->send_btn->setDisabled(true);
	ui->receive_btn->setDisabled(true);
	clock_time = 0;
	for(int i = 0; i < MAX_DATA; i++)
		data_array[i] = -1;
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::console_print(QString data)
{
	QString curr_data = ui->console->toPlainText();
	if(curr_data != "")
		ui->console->setText(curr_data + "\n" + data);
	else
		ui->console->setText(data);
}

void MainWindow::on_connect_btn_clicked()
{
	address = ui->address_lineEdit->text();
	if(address != "")
	{
#ifdef ARC_TYPE
		int fd = wiringPiI2CSetup(address.toInt());
		int tmp_data = wiringPiI2CRead(fd);
		if(tmp_data == -1)
		{
			console_print("Selected address : "+address+"; Module not Connected");
			statusBar()->showMessage(tr("Not Connected"));
			ui->send_btn->setDisabled(true);
			ui->receive_btn->setDisabled(true);
		}
		else
		{
			console_print("Selected address : "+address+"; Module connected succesfully");
			statusBar()->showMessage(tr("Connected Successfully"));
			ui->send_btn->setDisabled(false);
			ui->receive_btn->setDisabled(false);
		}
#endif
	}
}

void MainWindow::on_delay_btn_clicked()
{
	if(ui->time_lineEdit->text() == "")
		clock_time = 0;
	else
		clock_time = ui->time_lineEdit->text().toLongLong();
	console_print("Time delay : "+QString::number(clock_time)+" clock cycle(s)");
}

void MainWindow::on_send_btn_clicked()
{
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(address.toInt());

	for(int i = 0; i < index_val; i++)
	{
		console_print("Send Data : "+QString::number(data_array[i]));
		wiringPiI2CWrite(fd, data_array[i]);
		for(long long int t = 0; t < clock_time; t++);
	}
	//close(fd);
#endif
}

void MainWindow::on_receive_btn_clicked()
{
#ifdef ARC_TYPE
	int fd = wiringPiI2CSetup(address.toInt());
	int tmp_data = wiringPiI2CRead(fd);
	console_print("Received Data : "+QString::number(tmp_data));
	//close(fd);
#endif
}

void MainWindow::on_clean_btn_clicked()
{
	ui->console->setText("");
}

void MainWindow::on_data_add_btn_clicked()
{
	if(ui->data_lineEdit->text() != "")
	{
		QString tmp_data = "";
		int val = ui->data_lineEdit->text().toInt();

		data_array[index_val++] = val;

		for(int i = 0; i < index_val; i++)
			tmp_data += QString::number(data_array[i]) + ", ";

		ui->data_text_label->setText("<font style=\"font-size:14pt; font-weight:600; color:#006f00;\">Data: "+tmp_data+ "</font>");
		ui->data_lineEdit->setText("");
		ui->data_lineEdit->setFocus();
	}
}

void MainWindow::on_data_reset_btn_clicked()
{
	ui->data_text_label->setText("<font style=\"font-size:14pt; font-weight:600; color:#006f00;\">Data: </font>");
	ui->data_lineEdit->setText("");
	for(int i = 0; i < MAX_DATA; i++)
		data_array[i] = -1;
	index_val = 0;
}
