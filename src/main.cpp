#include <QApplication>
#include <QScreen>
#include <QFontDatabase>

#include "main.h"
#include "mainwindow.h"
#include "dashboard.h"
#include "settings.h"

static MainWindow *mainwindow_pt;
static dashboard *dashboardwindow_pt;
static settings *settings_pt;

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

void settings_window_show(int mode)
{
	if(mode)
	{
		settings_pt->show();
	}
	else
		settings_pt->hide();
}

int main(int argc, char *argv[])
{
	system("/usr/share/iAloy/scripts/iAloy-interface-up.sh");

	QApplication a(argc, argv);

	QFontDatabase::addApplicationFont("/usr/share/iAloy/fonts/DejaVuSerif.ttf");

	a.setFont(QFont("DejaVuSerif", 11, QFont::Normal, false));

	MainWindow mainwindow;
	dashboard dashboardwindow;
	settings settingswindow;

	mainwindow_pt = &mainwindow;
	dashboardwindow_pt = &dashboardwindow;
	settings_pt = &settingswindow;

	main_window_show(true);

	dashboard_window_show(false);

	return a.exec();
}
