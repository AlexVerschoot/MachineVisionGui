#include "EmergencyStop.h"
#include "ui_EmergencyStop.h"
#include "QTimer"
#include "unistd.h"



EmergencyStop::EmergencyStop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmergencyStop)
{
    ui->setupUi(this);
    ui->label_emergencytimer->setVisible(false);
    show();
    //QTimer::singleShot(100, this, SLOT(startSerialCommuncation()));
    exec();
}

EmergencyStop::~EmergencyStop()
{
    delete ui;
}

void EmergencyStop::newEmergency(){

}

void EmergencyStop::emergencyGone(){
    ui->label_stopInstructions->setText("Emergency averted. Please wait ");
    ui->label_emergencytimer->setVisible(true);


    QString text;
    for (int var = 0; var < 10; ++var) {
        text =(10-var)+" s";
        ui->label_emergencytimer->setText(text);
        sleep(1);
    }
}

