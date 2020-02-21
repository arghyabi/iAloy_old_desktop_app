#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "connect_init.h"

enum email_status
{
	SUPER_USER_EMAIL = 11,
	PERMANENT_USER_EMAIL = 10,
	INVALID_USER_EMAIL = 0
};

enum setUpLineEditStat
{
	GET_INPUT_EMAIL_MODE = 0,
	GET_INPUT_PROD_KEY_MODE,
	GET_INPUT_PASSWORD_MODE
};

enum prodKey_status
{
	PRODUCT_KEY_AVAILABLE = 0,
	PRODUCT_KEY_NOT_AVAILABLE,
	PRODUCT_KEY_INVALID
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public ialoy_web_api
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	void addBgImange();
	ialoy_web_api api;
	~MainWindow();

//private slots:

private slots:
	void on_SuEmailSubmit_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
