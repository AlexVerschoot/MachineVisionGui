#include "mainwindow.h"
#include <QApplication>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //show the interface
    w.showFullScreen();

    return a.exec();
}
