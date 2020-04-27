#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <QDialog>
#include <QThread>
#include <QLinkedList>

#include "ialoy_data.h"

namespace Ui {
class update_manager;
}

class update_manager : public QDialog, public ialoy_main_data
{
	Q_OBJECT
	QThread workerThread;
public:
	QString download_url;
	int download_size;
	QString latest_version;
	QString downloaded_version;

	explicit update_manager(QWidget *parent = 0);
	void init();

	void send_update_manager_request_type();
	void render_version_details();

	QString read_current_version();
	QString read_latest_version();
	void console_print(QString data);
	~update_manager();

signals:
	void fetch_update_status(bool);
	void download_update_tarball(QString, int, QString);
	void untar_download_file();


public slots:
	void dowload_already_complete_render();
	void fetch_update_status_need_update_render_slot(QLinkedList<struct version_details_node*>, bool, bool);
	void fetch_update_status_failed_render_slot();
	void download_progressbar_render_slot(int, int);
	void untar_download_file_render_slot();
	void finished_render_slot();
	void on_version_list_combo_box_currentIndexChanged(int index);

private slots:
	void on_update_btn_clicked();
	void on_cancel_btn_clicked();
	void on_restart_btn_clicked();

private:
	Ui::update_manager *ui;
	QLinkedList<struct version_details_node*> version_list_details;
	bool is_retry;
};

#endif // UPDATE_MANAGER_H
