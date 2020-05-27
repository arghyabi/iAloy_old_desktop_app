#ifndef MODULE_MANAGER_THREAD_H
#define MODULE_MANAGER_THREAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <QObject>
#include <QProcess>

#include "ialoy_data.h"

using namespace std;

class module_manager_thread : public QObject, public ialoy_main_data
{
	Q_OBJECT
public:
	explicit module_manager_thread(QObject *parent = nullptr);
	QProcess *mProcess;
	QString mOutputString;
	module_type module_type_flag;

signals:
	void start_burning();
	void start_burning_signal();
	void console_print_signal(QString);
	void burn_complete_signal();

public slots:
	void burn_module_slot(int, int);
	void start_burning_tmp_slot();
	void burning_module_tmp_slot();
	void burn_complete_tmp_slot(int);

private:
	string modify_code(string data, int address);
	string modify(int address);
	string generate_checksum(string checksum_input);
	string int_to_hex(int int_data);
	int hex_to_int(string hex_data);
};

#endif // MODULE_MANAGER_THREAD_H
