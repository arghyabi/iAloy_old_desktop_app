#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::console_print(QString data)
{
    QString curr_data = ui->console->toPlainText();
    if(curr_data != "")
        ui->console->setText(curr_data + "\n" + data);
    else
        ui->console->setText(data);
//    ui->console->verticalScrollBar()->setValue(ui->console->verticalScrollBar()->maximum());
}

void MainWindow::on_connect_btn_clicked()
{
    address = ui->address_lineEdit->text();
    console_print("Selected address : "+address);
}

void MainWindow::on_delay_btn_clicked()
{
    time = ui->time_lineEdit->text().toInt();
    console_print("Time delay : "+QString::number(time)+"Sec");
}

void MainWindow::on_send_btn_clicked()
{
    data = ui->data_lineEdit->text();
    console_print("Data to send : "+data);
    data_arr = convertStrtoArr(data);
    QString data_array_print;
    for(int i=0; i<15;i++)
        data_array_print += "\nData["+QString::number(i)+"] : "+*(data_arr+i);
    console_print(data_array_print);
}

void MainWindow::on_receive_btn_clicked()
{
    data = ui->data_lineEdit->text();
    console_print("Received Data : "+rec_data);
}

void MainWindow::on_clean_btn_clicked()
{
    ui->console->setText("");
}

int * MainWindow::convertStrtoArr(QString str)
{
    // get length of string str
    string string = str.toStdString();
    int str_length = str.length();
    int j = 0, i;
    int array[str_length] = {0};

    // Traverse the string
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ',')
            continue;
        if (str[i] == ' ')
            j++;
        else
            array[j] = array[j] * 10 + str[i].digitValue();
    }

    return array;
}
