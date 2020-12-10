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

	check_update_available = false;
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

		parse_fetched_data(val);

		current_version = read_current_version();
		cout << "current version:" << current_version.toStdString() << endl;
		cout << "latest version:" << latest_version.toStdString() << endl;

		if(check_update_available)
		{
			check_update_available = false;
			if(current_version == latest_version)
				emit check_update_available_response_for_parent_signal(false);
			else
				emit check_update_available_response_for_parent_signal(true);
		}
		else
		{
			write_latest_version(current_version, latest_version);

			if(current_version == latest_version)
				emit fetch_update_status_need_update(version_details_list, true, is_downloaded);
			else
				emit fetch_update_status_need_update(version_details_list, false, is_downloaded);
		}
	}
	);
}

void update_manager_thread::parse_fetched_data(QString version_info)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	if(version_info.left(1) == '1')
	{
		version_info = version_info.mid(1,-1);
		cout << "resp : " << version_info.toStdString() << endl;
	}

	// parsing data
	QJsonDocument jsonDocument = QJsonDocument::fromJson(version_info.toUtf8());
	QJsonObject jsonObject = jsonDocument.object();
	QJsonValue ArrayValue = jsonObject.value("version_list");
	QJsonArray version_Array = ArrayValue.toArray();

	if(!version_details_list.isEmpty())
		version_details_list.clear();

	bool latest_check_flag = true;
	foreach(const QJsonValue &version_data, version_Array)
	{
		struct version_details_node* version_details_nd = new struct version_details_node;

		version_details_nd->version = version_data.toObject().value("version").toString();
		version_details_nd->date = version_data.toObject().value("date").toString();
		version_details_nd->details = version_data.toObject().value("details").toString();
		version_details_nd->size = version_data.toObject().value("size").toString().toInt();
		version_details_nd->url = version_data.toObject().value("url").toString();

		version_details_list.append(version_details_nd);

		if(latest_check_flag)
		{
			this->latest_version = version_details_nd->version;
			latest_check_flag = false;
		}
	}
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

void update_manager_thread::fetch_update_status_slot(bool is_downloaded)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	this->is_downloaded = is_downloaded;
	NetworkRequest.setUrl(QString::fromStdString("https://ialoy.arghyabiswas.com/update_manager?upmc=1"));
	NetworkManager->get(NetworkRequest);
}

void update_manager_thread::check_update_available_slot()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	check_update_available = true;
	fetch_update_status_slot(false);
	cout << "Latest version : " << latest_version.toStdString() << "\nCurrent version : " << current_version.toStdString() << endl;
}

void update_manager_thread::download_update_tarball_slot(QString download_url, int download_size, QString selected_ver)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	cout << "url :" << download_url.toStdString() << endl;
	cout << "total_size:" << download_size << endl;

	total_size = download_size;
	selected_version = selected_ver;

	system("rm -rf /usr/share/iAloy/.temp/*");
	NetworkDownloadRequest.setUrl(download_url);
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
	cout << "Downloaded : " << read << " of " << total_size << endl;
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
	system("echo " + selected_version.toUtf8() + " > /usr/share/iAloy/.temp/done");
	emit untar_dowload_tarball_complete();
}
