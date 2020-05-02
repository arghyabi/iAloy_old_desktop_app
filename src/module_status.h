#ifndef MODULE_STATUS_H
#define MODULE_STATUS_H

#include <QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QSignalMapper>

#include"ialoy_data.h"
#include "i2c_data.h"

namespace Ui {
class module_status;
}

class module_status : public QDialog, public ialoy_main_data
{
	Q_OBJECT

public:
	QPushButton *btn;
	QSignalMapper *signalMapper;

	explicit module_status(QWidget *parent = nullptr);
	~module_status();

	void init();
	QJsonArray get_json_array_from_response(string);
	void render_module_status_state(string);
	bool device_controller_api_response_parse(string);

signals:
	void read_all_i2c_module_state_signal(int*);

private slots:
	void button_clicked_slot(QString);


private:
	Ui::module_status *ui;
};

#endif // MODULE_STATUS_H
