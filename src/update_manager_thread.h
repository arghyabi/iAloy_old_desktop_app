#ifndef UPDATE_MANAGER_THREAD_H
#define UPDATE_MANAGER_THREAD_H

#include <QObject>
#include <QDataStream>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class update_manager_thread : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager *NetworkManager, *NetworkDownloadManager;
	QNetworkRequest NetworkRequest, NetworkDownloadRequest;
	QNetworkReply *webResponse;
	QString des = "/usr/share/iAloy/.temp/iAloy_update.zip";
	QString latest_version;
	QString size;
	QString url;
	QString read_current_version();
	void write_latest_version(QString current_ver, QString latest_ver);
	void purse_fetched_data(QString val);

public:
	explicit update_manager_thread(QObject *parent = nullptr);

signals:
	void fetch_update_status_need_update();
	void fetch_update_status_already_uptodate();
	void fetch_update_status_failed();
	void download_progressbar_update_signal(int, int);
	void download_update_tarball_complete();
	void untar_dowload_tarball_complete();

public slots:
	void fetch_update_status_slot();
	void download_update_tarball_slot();

	void error(QNetworkReply::NetworkError err);
	void updateProgress(qint64 read, qint64 total);
	void finished();

	void untar_dowload_tarball_slot();
};

#endif // UPDATE_MANAGER_THREAD_H
