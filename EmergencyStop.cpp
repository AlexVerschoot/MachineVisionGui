#include "EmergencyStop.h"
#include "ui_EmergencyStop.h"
#include "QTimer"
#include "unistd.h"
#include "iostream"



EmergencyStop::EmergencyStop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmergencyStop)
{
    ui->setupUi(this);
    //QTimer::singleShot(100, this, SLOT(startSerialCommuncation()));
}

EmergencyStop::~EmergencyStop()
{
    delete ui;
}

void EmergencyStop::newEmergency(){
    //show();
    exec();
}

void EmergencyStop::emergencyGone(MotorControllerSec * motorController){
    //if(stopping ==0){
    //    stopping = 1;
    motorController->initialize();


    QString text;
    for (int var = 0; var < 5; ++var) {
        std::cout<<var<<std::endl;
        sleep(1);
    }


    motorController->initialize();



    QTimer::singleShot(1, this, SLOT(accept()));
    //}
}

