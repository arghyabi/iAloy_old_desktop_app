#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <QDialog>
#include <QThread>

namespace Ui {
class update_manager;
}

class update_manager : public QDialog
{
	Q_OBJECT
	QThread workerThread;
public:
	explicit update_manager(QWidget *parent = 0);
	void init();
	QString read_current_version();
	QString read_latest_version();
	void console_print(QString data);
	~update_manager();

signals:
	void fetch_update_status();
	void download_update_tarball();
	void untar_download_file();

public slots:
	void dowload_already_complete_render();
	void fetch_update_status_need_update_render_slot();
	void fetch_update_status_already_uptodate_render_slot();
	void fetch_update_status_failed_render_slot();
	void download_progressbar_render_slot(int, int);
	void untar_download_file_render_slot();
	void finished_render_slot();

private slots:
	void on_update_btn_clicked();
	void on_cancel_btn_clicked();
	void on_restart_btn_clicked();

private:
	Ui::update_manager *ui;
};

#endif // UPDATE_MANAGER_H
