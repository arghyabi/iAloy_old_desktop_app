#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <QtWidgets/QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

#include <QDialog>
#include "i2c_data.h"
#include "module_manager_thread.h"
#include "dashboard.h"

namespace Ui {
class module_manager;
}

class module_manager : public QDialog, public ialoy_main_data
{
	Q_OBJECT

public:
	QPushButton *add_btn;
	QSignalMapper *signalMapper, *linkSignalMapper;
	QProcess *mmt_Process;
	QString mOutputString;
	int selected_mod_address;

	QHBoxLayout *hLayoutLinkModule;
	QSpacerItem *hSpacerLinkModuleLeft, *hSpacerLinkModuleMid, *hSpacerLinkModuleRight, *verticalSpacer;
	QLabel *CountLabel, *ModAddLabel, *linkingLabel;
	QPushButton *LinkButton;
	QWidget *scrollAreaWidgetContents;

	int link_mod_add_array[117][2];
	int link_button_length;
	int module_index;

	explicit module_manager(QWidget *parent = nullptr);
	~module_manager();

	void send_add_module_api_request();
	void console_print(QString data);
	void clearLayout(QLayout* layout);

public slots:
	void init(QLinkedList<btn_node*>);
	void button_clicked_slot(QString);

	void start_burning_slot();
	void burning_module_slot(QString);
	void burn_complete_slot();
	void link_button_clicked_slot(QString);
	void add_new_module_api_resp_slot(string);
	void render_link_button_list();

private slots:
	void on_burn_button_clicked();
	void on_refresh_tool_button_clicked();

signals:
	void read_all_i2c_module_state_signal(int*);
	void burn_module_signal(int);
	void render_link_button_list_signal();
	void refresh_module_manager_signal();
	void add_new_module_api_request_signal(string);

private:
	Ui::module_manager *ui;
};

#endif // MODULE_MANAGER_H
