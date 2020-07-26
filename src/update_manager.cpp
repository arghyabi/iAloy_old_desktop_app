#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "update_manager.h"
#include "update_manager_thread.h"
#include "ui_update_manager.h"

using namespace std;

update_manager::update_manager(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::update_manager)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->setupUi(this);

#ifdef ARC_TYPE
	is_retry = false;
	update_manager_thread *thread_obj = new update_manager_thread(this);
	thread_obj->moveToThread(&workerThread);

	connect(this, SIGNAL(fetch_update_status(bool)), thread_obj, SLOT(fetch_update_status_slot(bool)));
	connect(thread_obj, SIGNAL(fetch_update_status_need_update(QLinkedList<struct version_details_node*>, bool, bool)), this, SLOT(fetch_update_status_need_update_render_slot(QLinkedList<struct version_details_node*>, bool, bool)));
	connect(thread_obj, SIGNAL(fetch_update_status_failed()), this, SLOT(fetch_update_status_failed_render_slot()));
	connect(this, SIGNAL(download_update_tarball(QString, int, QString)), thread_obj, SLOT(download_update_tarball_slot(QString, int, QString)));
	connect(thread_obj, SIGNAL(download_progressbar_update_signal(int, int)), this, SLOT(download_progressbar_render_slot(int, int)));
	connect(thread_obj, SIGNAL(download_update_tarball_complete()), this, SLOT(untar_download_file_render_slot()));
	connect(this, SIGNAL(untar_download_file()), thread_obj, SLOT(untar_dowload_tarball_slot()));
	connect(thread_obj, SIGNAL(untar_dowload_tarball_complete()), this, SLOT(finished_render_slot()));

	ui->version_status->setText("Connecting...");
#else
	ui->version_status->setText("Wrong CPU Architecture Found...");
#endif

	ui->update_btn->hide();
	ui->console_area->hide();
	ui->download_progress->hide();
	ui->progressBar->hide();
	ui->restart_btn->hide();
	ui->version_list_combo_box->hide();
	ui->version_details->hide();
}

void update_manager::init()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QFile file;
	file.setFileName(QString::fromStdString("/usr/share/iAloy/.temp/done"));
	if(file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		file.close();
		cout << "Download already complete" << endl;
		dowload_already_complete_render();
		emit fetch_update_status(true);
	}
	else
	{
		cout << "Need to download" << endl;
		emit fetch_update_status(false);
	}
}

update_manager::~update_manager()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	delete ui;
}

void update_manager::dowload_already_complete_render()
{
	ui->version_status->setText("Downlaod completed, Please restart the system");
	ui->restart_btn->show();
}

QString update_manager::read_current_version()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QFile file;
	file.setFileName(QString::fromStdString("/usr/share/iAloy/.conf/version_info.json"));

	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString val = file.readAll();
	file.close();

	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject sett2 = d.object();

	return QString::fromStdString(sett2["current_version"].toString().toStdString());
}

QString update_manager::read_latest_version()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QFile file;
	file.setFileName(QString::fromStdString("/usr/share/iAloy/.conf/version_info.json"));

	file.open(QIODevice::ReadOnly | QIODevice::Text);
	QString val = file.readAll();
	file.close();

	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject sett2 = d.object();

	return QString::fromStdString(sett2["latest_version"].toString().toStdString());
}

void update_manager::console_print(QString data)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QString curr_data = ui->console_area->toPlainText();
	if(curr_data != "")
		ui->console_area->setText(curr_data + "\n" + data);
	else
		ui->console_area->setText(data);
}

void update_manager::render_version_details()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	struct version_details_node* version_details_nd;
	foreach (version_details_nd, version_list_details)
	{
		if(ui->version_list_combo_box->currentText() == version_details_nd->version)
		{
			ui->version_details->setText("<b>Details : </b>"+version_details_nd->details);
			download_size = version_details_nd->size;
			download_url = version_details_nd->url;
			latest_version = version_details_nd->version;
			break;
		}
	}
	ui->version_details->show();
}

void update_manager::fetch_update_status_need_update_render_slot(QLinkedList<struct version_details_node*> version_list, bool is_updated, bool is_downloaded)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	is_retry = false;
	ui->update_btn->setText("Download");
	ui->restart_btn->hide();
	ui->update_btn->show();
	ui->console_area->show();
	ui->version_status->setText(read_current_version()+" > ");

	ui->version_list_combo_box->clear();
	struct version_details_node* version_details_nd;
	version_list_details = version_list;
	bool start_flag = true;

	if(is_downloaded)
	{
		// downloaded_version should be initialised from done file with proper format
		QFile done_file;
		done_file.setFileName(QString::fromStdString("/usr/share/iAloy/.temp/done"));

		done_file.open(QIODevice::ReadOnly | QIODevice::Text);
		downloaded_version = done_file.readAll();
		//need to remove the line end char that is autometically added by the system
		downloaded_version = downloaded_version.mid(0, downloaded_version.length()-1);
		done_file.close();
		cout << "Done version : \"" << downloaded_version.toStdString() << "\"" << endl;
	}

	foreach (version_details_nd, version_list)
	{
		if(start_flag)
		{
			download_size = version_details_nd->size;
			download_url = version_details_nd->url;
			latest_version = version_details_nd->version;
			ui->version_details->setText("<b>Details : </b>"+version_details_nd->details);
			console_print("Current Version: " + read_current_version());
			console_print("Latest Version: " + latest_version);
			start_flag = false;
		}
		if(is_downloaded && downloaded_version == version_details_nd->version)
		{
			cout << "Done version in render : " << downloaded_version.toStdString() << endl;
			continue;
		}
		if(version_details_nd->version == read_current_version())
			continue;
		ui->version_list_combo_box->addItem(version_details_nd->version);
	}
	ui->version_list_combo_box->show();

	render_version_details();
	ui->progressBar->setTextVisible(true);
	ui->progressBar->setFormat("Overall: 0%");
	ui->progressBar->setAlignment(Qt::AlignCenter);
	ui->download_progress->setTextVisible(true);
	ui->download_progress->setFormat("Download: 0%");
	ui->download_progress->setAlignment(Qt::AlignCenter);
	ui->progressBar->setValue(0);
	ui->download_progress->setValue(0);
	ui->download_progress->show();
	ui->progressBar->show();

	if(is_downloaded)
	{
		ui->update_status->setText("<font size=4 color='#1963dd'><b>"+downloaded_version+"</b> already downloaded.<br>Please restart to apply changes.</font>");
		ui->restart_btn->show();
	}
	else
	{
		if(is_updated)
			ui->update_status->setText("<font size=4 color='#128d2e'>Already upto date.</font>");
		else
			ui->update_status->setText("<font size=4 color='#f94400'>Update available</font>");
	}
	ui->update_status->show();
}

void update_manager::fetch_update_status_failed_render_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->version_status->setText("<font size=4 color='#f94400'>Can't fetch update, please try again.</font>");
	ui->update_btn->setText("Retry");
	ui->update_btn->show();
	ui->update_status->hide();
	ui->restart_btn->hide();
	is_retry = true;
}

void update_manager::download_progressbar_render_slot(int read, int total)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	float percent = (read*100)/total;
	ui->download_progress->setValue(percent);
	ui->download_progress->setFormat("Downloading: " + QString::number(percent) + "%");
	percent = percent/2;
	ui->progressBar->setValue(percent);
	ui->progressBar->setFormat("Overall: " + QString::number(percent)+"%");
}

void update_manager::untar_download_file_render_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	console_print("Download Complete.");
	console_print("Extracting...");
	emit untar_download_file();
}

void update_manager::finished_render_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->progressBar->setValue(100);
	ui->progressBar->setFormat("Overall: 100%");
	ui->download_progress->setFormat("Download completed");
	ui->update_btn->show();
	console_print("Extracting is complete.");
	console_print("Please restart the system to complete the installation process.");
	ui->update_status->setText("<font size=4 color='#1963dd'>Download completed.<br>Restart to apply changes.</font>");
	ui->restart_btn->show();
}

void update_manager::on_update_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	if(!is_retry)
	{
		ui->update_btn->hide();
		console_print("\nSelected version: " + latest_version);
		console_print("Downloading...");
		emit download_update_tarball(download_url, download_size, latest_version);
	}
	else
		this->init();
}


void update_manager::on_cancel_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	ui->update_btn->hide();
	ui->console_area->setText("");
	ui->console_area->hide();
	ui->download_progress->hide();
	ui->progressBar->hide();
	ui->restart_btn->hide();
	ui->version_list_combo_box->hide();
	ui->version_list_combo_box->clear();
	ui->version_details->hide();
	this->close();
}

void update_manager::on_restart_btn_clicked()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	system("reboot now");
	console_print("Restarting...");
}

void update_manager::on_version_list_combo_box_currentIndexChanged(int index)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	render_version_details();
}
