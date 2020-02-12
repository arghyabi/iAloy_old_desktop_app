#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addBgImange()
{
    QPixmap bkgnd("back.jpg");
    bkgnd = bkgnd.scaled(this->width(),this->height());
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_push_Button_2_clicked()
{
    ui->label123->setText("albalchal");
}
