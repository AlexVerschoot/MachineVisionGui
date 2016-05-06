#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include "EmergencyStop.h"
#include <wiringPi.h>


void emergencyStop(void) {
    EmergencyStop stoppie;
    //stoppie.newEmergency();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    wiringPiISR (28, INT_EDGE_FALLING, &emergencyStop);
    //show the interface
    w.showFullScreen();

    return a.exec();
}
