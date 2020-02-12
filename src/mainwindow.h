#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void on_pushButton_clicked();

    void on_push_Button_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
