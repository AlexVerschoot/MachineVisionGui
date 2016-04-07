#include "motorcontrollerstartdialog.h"
#include "ui_motorcontrollerstartdialog.h"
#include "QTimer"

MotorControllerStartDialog::MotorControllerStartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MotorControllerStartDialog)
{
    ui->setupUi(this);
    show();
    QTimer::singleShot(1, this, SLOT(startSerialCommuncation()));
    exec();



}

MotorControllerStartDialog::~MotorControllerStartDialog()
{
    delete ui;
}

void MotorControllerStartDialog::startSerialCommuncation(){
    SerialPort serialPort;
    std::cout << "communication enabled" << std::endl;

    //init for when you start the controller
    for (int var = 0; var < 15; ++var) {
        serialPort.readPort();
    }
    ui->label_motorInstructions->setText("Motor initializing...");
    ui->progress_started->setValue(90);

    //write drive enable
    sleep(1);
    unsigned char temp[] = "SRF+ \r\n";
    serialPort.writePort(temp);
    sleep(1);
    ui->label_motorInstructions->setText("Motor ready.");
    ui->progress_started->setValue(100);
    QTimer::singleShot(1, this, SLOT(accept()));
}
