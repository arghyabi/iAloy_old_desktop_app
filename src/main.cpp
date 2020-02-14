#include "mainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QSize size = qApp->screens()[0]->size();
    w.resize(size);
    w.addBgImange();
    w.showFullScreen();

    w.show();
//    print_name();
    return a.exec();
}
