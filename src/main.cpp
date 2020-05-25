#include <QApplication>
#include <QScreen>
#include <QFontDatabase>

#include "main.h"
#include "mainwindow.h"
#include "dashboard.h"
#include "update_manager.h"

static MainWindow *mainwindow_pt;
static dashboard *dashboardwindow_pt;
static update_manager *update_manager_pt;

void main_window_show(int mode)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ <<"    mode :" << mode << endl;
	if(mode)
	{
		QSize size = qApp->screens()[0]->size();
		mainwindow_pt->resize(size);
		//mainwindow_pt->addBgImage();
		mainwindow_pt->showFullScreen();
		mainwindow_pt->show();
	}
	else
		mainwindow_pt->hide();
}

void mainwindow_reset_on_logout()
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	mainwindow_pt->mainwindow_reset_on_logout();
}

void dashboard_window_show(bool mode)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ <<"    mode :" << mode << endl;
	if(mode)
	{
		QSize size = qApp->screens()[0]->size();
		dashboardwindow_pt->resize(size);
		// dashboardwindow_pt->addBgImage(); // TODO: Solve the background image problem
		dashboardwindow_pt->showFullScreen();
		dashboardwindow_pt->show();
		dashboardwindow_pt->init();
	}
	else
		dashboardwindow_pt->hide();
}

void update_manager_window_show(int mode)
{
	cout << ">>>> " << __PRETTY_FUNCTION__ <<"    mode :" << mode << endl;
	if(mode)
	{
		update_manager_pt->show();
		update_manager_pt->init();
	}
	else
		update_manager_pt->hide();
}

int main(int argc, char *argv[])
{
	cout << ">>>> " << __PRETTY_FUNCTION__ << endl;
	system("/usr/share/iAloy/scripts/iAloy-interface-up.sh");

	QApplication a(argc, argv);

	QFontDatabase::addApplicationFont("/usr/share/iAloy/fonts/DejaVuSerif.ttf");

	a.setFont(QFont("DejaVuSerif", 11, QFont::Normal, false));

	MainWindow mainwindow;
	dashboard dashboardwindow;
	update_manager updateManager;

	mainwindow_pt = &mainwindow;
	dashboardwindow_pt = &dashboardwindow;
	update_manager_pt = &updateManager;

	main_window_show(true);

	dashboard_window_show(false);

	return a.exec();
}
