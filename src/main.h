#ifndef MAINFILE_H
#define MAINFILE_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

#include "mainwindow.h"
#include "dashboard.h"

void main_window_show(int mode);
void dashboard_window_show(bool mode);
void settings_window_show(int mode);
void update_manager_window_show(int mode);

void mainwindow_reset_on_logout();

#endif // MAIN_H
