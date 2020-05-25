#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ialoy_data.h"
#include "dashboard.h"

namespace Ui {
	class settings;
}

enum updatePasswordState
{
	INITIALIZED = 0,
	READY_TO_VERIFY,
	READY_TO_SET_NEW,
};

class settings : public QDialog, public ialoy_main_data
{
	Q_OBJECT

public:
	updatePasswordState updatePasswordState_flag;
	QString new_password;
	int password_matched;
	explicit settings(QWidget *parent = 0);
	~settings();

signals:
	void verify_password_signal(string);
	void update_password_signal(string);

private slots:
	void render_update_password();
	void on_passwordUpdatePushButton_clicked();
	void on_updateCancelpushButton_clicked();
	void on_pushButton_10_clicked();

public slots:
	void init(int mode);
	void verify_password_response_slot(bool);
	void update_password_response_slot(bool);

private:
	Ui::settings *ui;
};

#endif // SETTINGS_H
