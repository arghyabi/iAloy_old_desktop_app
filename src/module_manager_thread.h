#ifndef MODULE_MANAGER_THREAD_H
#define MODULE_MANAGER_THREAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <QObject>

using namespace std;

class module_manager_thread : public QObject
{
	Q_OBJECT
public:
	explicit module_manager_thread(QObject *parent = nullptr);

signals:
	void burn_complete_signal();

public slots:
	void burn_module_slot(int);

private:
	string modify_code(string data, int address);
	string modify(int address);
	string generate_checksum(string checksum_input);
	string int_to_hex(int int_data);
	int hex_to_int(string hex_data);
};

#endif // MODULE_MANAGER_THREAD_H
