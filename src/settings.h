#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QSignalMapper>

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

	QLabel *other_user_name_label;
	QLabel *other_user_email_label;
	QPushButton *other_user_login_button;
	QPushButton *other_user_remove_button;
	QHBoxLayout *horizontalLayout;
	QHBoxLayout *btnHorizontalLayout;
	QVBoxLayout *verticalLayout;
	QSpacerItem *verticalSpacer;
	QSpacerItem *horizontalSpacer;
	QSignalMapper *signalMapper;

	updatePasswordState updatePasswordState_flag;
	QString new_password;
	int password_matched;
	explicit settings(QWidget *parent = 0);
	void render_other_user_details(string);
	void clearLayout(QLayout* layout);

	~settings();

signals:
	void verify_password_signal(string);
	void update_password_signal(string);
	void get_other_users_info_signal();
	void dashboard_request_other_user_login_signal(QString);

private slots:
	void render_update_password();
	void on_passwordUpdatePushButton_clicked();
	void on_updateCancelpushButton_clicked();
	void on_pushButton_10_clicked();

public slots:
	void init(int mode);
	void verify_password_response_slot(bool);
	void update_password_response_slot(bool);
	void get_other_user_info_response_slot(string);
	void other_user_login_button_clicked_slot(QString);

private:
	Ui::settings *ui;
};

#endif // SETTINGS_H
