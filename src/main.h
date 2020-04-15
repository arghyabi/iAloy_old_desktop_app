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

#endif // MAIN_H
