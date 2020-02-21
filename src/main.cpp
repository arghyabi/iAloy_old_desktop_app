#include <QApplication>
#include <QScreen>

#include "main.h"
#include "mainwindow.h"
#include "dashboard.h"

static MainWindow *mainwindow_pt;
static dashboard *dashboardwindow_pt;

void main_window_show(int mode)
{
	if(mode)
	{
		QSize size = qApp->screens()[0]->size();
		mainwindow_pt->resize(size);
		mainwindow_pt->addBgImage();
		mainwindow_pt->showFullScreen();
		mainwindow_pt->show();
	}
	else
		mainwindow_pt->hide();
}

void dashboard_window_show(int mode)
{
	if(mode)
	{
		QSize size = qApp->screens()[0]->size();
		dashboardwindow_pt->resize(size);
		dashboardwindow_pt->addBgImage();
		dashboardwindow_pt->showFullScreen();
		dashboardwindow_pt->show();
	}
	else
		dashboardwindow_pt->hide();
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow mainwindow;
	dashboard dashboardwindow;

	mainwindow_pt = &mainwindow;
	dashboardwindow_pt = &dashboardwindow;

	main_window_show(true);

	dashboard_window_show(false);

	return a.exec();
}
