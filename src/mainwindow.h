#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "connect_init.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void addBgImange();
    ~MainWindow();

//private slots:

private slots:
    void on_SuEmailSubmit_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
