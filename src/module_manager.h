#ifndef MODULE_MANAGER_H
#define MODULE_MANAGER_H

#include <QtWidgets/QPushButton>
#include <QSignalMapper>
#include <QMessageBox>

#include <QDialog>
#include "ialoy_data.h"
#include "i2c_data.h"
#include "module_manager_thread.h"

namespace Ui {
class module_manager;
}

class module_manager : public QDialog, public ialoy_main_data
{
	Q_OBJECT

public:
	QPushButton *add_btn;
	QSignalMapper *signalMapper;
	int selected_mod_address;
	explicit module_manager(QWidget *parent = nullptr);
	~module_manager();

	void console_print(QString data);

public slots:
	void init(QLinkedList<btn_node*>);
	void button_clicked_slot(QString);
	void burn_complete_slot();

private slots:
	void on_burn_button_clicked();

signals:
	void read_all_i2c_module_state_signal(int*);
	void burn_module_signal(int);

private:
	Ui::module_manager *ui;
};

#endif // MODULE_MANAGER_H
