#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class settings;
}

class settings : public QDialog
{
	Q_OBJECT

public:
	explicit settings(QWidget *parent = 0);
	~settings();

	void init();
	void set_font_name_and_size();

private slots:
	void on_buttonBox_accepted();
	void on_pushButton_clicked();

private:
	Ui::settings *ui;
};

#endif // SETTINGS_H
