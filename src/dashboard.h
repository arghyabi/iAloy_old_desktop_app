#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include<QString>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

using namespace std;

namespace Ui {
class dashboard;
}

class dashboard : public QMainWindow
{
	Q_OBJECT

public:
	explicit dashboard(QWidget *parent = 0);
	void addBgImage();
	void render_dashboard_room_btn();
	~dashboard();

private slots:

private:
	Ui::dashboard *ui;
};

#endif // DASHBOARD_H
