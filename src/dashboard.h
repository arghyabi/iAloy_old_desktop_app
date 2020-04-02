#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QMainWindow>
#include <iostream>

#include "device_controller_api.h"
#include "ialoy_data.h"

namespace Ui {
class dashboard;
}

class dashboard : public QDialog, public ialoy_main_data
{
	Q_OBJECT

public:
	explicit dashboard(QWidget *parent = nullptr);
	void addBgImage();
	~dashboard();

private:
	Ui::dashboard *ui;
};

#endif // DASHBOARD_H
