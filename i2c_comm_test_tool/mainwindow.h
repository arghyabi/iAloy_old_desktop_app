#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <bits/stdc++.h>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString address, data, rec_data;
    int time;
    int *data_arr;
    int * convertStrtoArr(QString);
    MainWindow(QWidget *parent = nullptr);
    void console_print(QString);
    ~MainWindow();

private slots:
    void on_connect_btn_clicked();

    void on_delay_btn_clicked();

    void on_send_btn_clicked();

    void on_receive_btn_clicked();

    void on_clean_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
