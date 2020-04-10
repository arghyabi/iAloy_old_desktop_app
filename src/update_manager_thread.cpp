#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "update_manager_thread.h"

using namespace std;

update_manager_thread::update_manager_thread(QObject *parent) : QObject(parent)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;

	NetworkManager = new QNetworkAccessManager();
	QObject::connect(NetworkManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)
		{
		if (reply->error())
		{
			cout << "Error : " << reply->errorString().toStdString() << endl;
			emit fetch_update_status_failed();
			return;
		}

		QString val = reply->readAll();

		purse_fetched_data(val);
		QString current_version = read_current_version();
		cout << "current version:" << current_version.toStdString() << endl;
		cout << "latest version:" << latest_version.toStdString() << endl;

		write_latest_version(current_version, latest_version);

		if(current_version == latest_version)
			emit fetch_update_status_already_uptodate();
		else
			emit fetch_update_status_need_update();
	}
	);
}

void update_manager_thread::purse_fetched_data(QString val)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(val.left(1) == '1')
	{
		val = val.mid(1,-1);
		cout << "resp : " << val.toStdString() << endl;
	}
	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject sett2 = d.object();

	this->latest_version = QString::fromStdString(sett2["version"].toString().toStdString());
	this->size = QString::fromStdString(sett2["size"].toString().toStdString());
	this->url = QString::fromStdString(sett2["url"].toString().toStdString());
}

QString update_manager_thread::read_current_version()
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

void update_manager_thread::write_latest_version(QString current_ver, QString latest_ver)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QFile file;
	file.setFileName(QString::fromStdString("/usr/share/iAloy/.conf/version_info.json"));

	QString data = "\
{\n\
	\"current_version\" : \"" + current_ver + "\",\n\
	\"latest_version\" : \"" + latest_ver + "\"\n\
}\
";

	file.open(QIODevice::WriteOnly | QIODevice::Text);
	file.write(data.toUtf8());
	file.close();
}

void update_manager_thread::fetch_update_status_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	NetworkRequest.setUrl(QString::fromStdString("http://ialoy.arghyabiswas.com/desktop_api/update_package_req_manager.php?upmc=0"));
	NetworkManager->get(NetworkRequest);
}

void update_manager_thread::download_update_tarball_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	system("rm -rf /usr/share/iAloy/.temp/*");
	NetworkDownloadRequest.setUrl(this->url);
	NetworkDownloadManager = new QNetworkAccessManager();
	webResponse = NetworkDownloadManager->get(NetworkDownloadRequest);
	connect(webResponse, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
	connect(webResponse, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
	connect(webResponse, SIGNAL(finished()), this, SLOT(finished()));
}


void update_manager_thread::error(QNetworkReply::NetworkError err)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "error: " << err << endl;
	webResponse->deleteLater();
}

void update_manager_thread::updateProgress(qint64 read, qint64 total)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "Downloaded : " << read << " of " << total << endl;
	int total_size = this->size.toInt();
	emit download_progressbar_update_signal((int)read, total_size);
}

void update_manager_thread::finished()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QByteArray b = webResponse->readAll();
	QFile file(des);
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);
	out << b;
	webResponse->deleteLater();
	emit download_update_tarball_complete();
}

void update_manager_thread::untar_dowload_tarball_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	QString unzip_command = "unzip " + this->des + " -d /usr/share/iAloy/.temp/";
	cout << "unzip command:" << unzip_command.toStdString() << endl ;
	system(unzip_command.toUtf8());
	system("rm -rf " + this->des.toUtf8());
	system("touch /usr/share/iAloy/.temp/done");
	emit untar_dowload_tarball_complete();
}
