#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "EmergencyStop.h"
#include <wiringPi.h>

MainWindow * w;



void emergencyStop(void) {
    w->emergencyStopPressed();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    wiringPiSetup () ;
    wiringPiISR (28, INT_EDGE_FALLING, &emergencyStop);
    w = new MainWindow();
    //show the interface
    w->showFullScreen();

    return a.exec();
}
