#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <wiringPi.h>


//TODO remove the namespaces and do everything individually
using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //wiringPiSetup () ;
    /*pinMode (28, INPUT) ;
    //for when it starts up with the button pressed
    if(!digitalRead(28)){
        emergencyStopPressed();
    }*/
    //set up the interrupts
    //wiringPiISR (28, INT_EDGE_FALLING, &MainWindow::emergencyStopPressed);
    //a timer that counts every second
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(emergencyStopReleased()));
    timer.setInterval(interval);
    timer.start();
    QTimer::singleShot(100, this, SLOT(startMotor()));
    //QTimer::singleShot(1000, this, SLOT(emergencyStopReleased()));

    ardo = new ArduinoCom();

    stoppie = new EmergencyStop();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_launcherStartButton_clicked()
{


    //TODO include some serial communication with the arduino
    std::cout<<"launcher should now start"<<std::endl;

    mainCamera->startCamera();
    ardo->send(0);
}

void MainWindow::on_launcherStopButton_clicked()
{
    //TODO include some serial communication with the arduino
    std::cout<<"launcher should now stop"<<std::endl;


    //TODO stop the camera detection
    mainCamera->stopCamera();
    ardo->send(1);
}

void MainWindow::on_launcherSpeedControl_valueChanged(int value)
{
    //TODO include some serial communication with the arduino
    std::cout<<"speed : " <<value<<std::endl;
    ardo->send(2, value);

}

void MainWindow::updateTime()
{
    runningTime ++;
    //the arg is number, amount of numbers, decimal, what number
    //this updates the status time
    QString number = QString("%1:").arg((runningTime/3600), 2, 10, QChar('0')) + QString("%1:").arg((runningTime/60%60), 2, 10, QChar('0'))+QString("%1").arg(runningTime%60, 2, 10, QChar('0'));
    ui->label_activeTime->setText(number);


    //the cpu activity
    //TODO put this in a library, improve the code so i can better reuse it in future programs
    char buf[100];
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 4; ++y) {
            a[x][y]=b[x][y];
        }
    }


    FILE * pFile2 = fopen ("/proc/stat" , "r");
    for (int var = 0; var < 5; ++var) {
        if (fgets(buf, sizeof buf, pFile2) != NULL){
            sscanf(buf,"%*s %Lf %Lf %Lf %Lf",&b[var][0],&b[var][1],&b[var][2],&b[var][3]);
        }
    }
    fclose(pFile2);
    for (int var = 0; var < 5; ++var) {
        loadavg = ((b[var][0]+b[var][1]+b[var][2]) - (a[var][0]+a[var][1]+a[var][2])) / ((b[var][0]+b[var][1]+b[var][2]+b[var][3]) - (a[var][0]+a[var][1]+a[var][2]+a[var][3]));

        switch (var) {
        case 1:
            ui->progress_cpu0->setValue(loadavg*100);
            break;
        case 2:
            ui->progress_cpu1->setValue(loadavg*100);
            break;
        case 3:
            ui->progress_cpu2->setValue(loadavg*100);
            break;
        case 4:
            ui->progress_cpu3->setValue(loadavg*100);
            break;
        default:
            ui->progress_cpuTotal->setValue(loadavg*100);
            break;
        }
    }

    //the temperature
    int tempe;
    FILE * pFile3 = fopen ("/sys/class/thermal/thermal_zone0/temp" , "r");
    if (fgets(buf, sizeof buf, pFile3) != NULL){
        sscanf(buf,"%i",&tempe);
        QString number ;//= number.number(tempe/1000);
        ui->label_temperature->setText(number.number(tempe/1000)+" °C");
    }else{
        std::cout<<"could not read temp"<<std::endl;
    }
    fclose(pFile3);

    //the framerate
    long framerateNew = mainCamera->getFrames();
    ui->label_framerate->setText(number.number((framerateNew-framerateOld)*(1000/interval))+" fps");
    framerateOld = framerateNew;

    //the image
    //TODO put the save location in a more central place"
    std::string s = "/home/pi/Pictures/motion_detected_"+std::to_string(mainCamera->getLastDetected())+".jpg";
    //cout << s << endl;
    const char * filename = s.c_str();
    QPixmap pix(filename);
    ui->labelPicture->setPixmap(pix);

    //the amount detected
    ui->label_sizeLarge->setText(number.number(mainCamera->getAmountofBigs()));
    ui->label_sizeMedium->setText(number.number(mainCamera->getAmountofMediums()));
    ui->label_sizeSmall->setText(number.number(mainCamera->getAmountofSmalls()));
    ui->label_sizeAll->setText(number.number(mainCamera->getAmountofSmalls()+mainCamera->getAmountofMediums()+mainCamera->getAmountofBigs()));


}


void MainWindow::on_servoMotorControl_valueChanged(int value)
{
    motorController->gotoPosition(value);
}

void MainWindow::startMotor(){
    //TODO make a settings menu where this can be redone
    motorController = new MotorControllerSec();

    mainCamera = new CameraMain(motorController);


}

/*
void MainWindow::emergencyStop(){
    if(!digitalRead(28)){
        if(!emergency){
            emergency = 1;
            std::cout<<"emergency stop detected"<<std::endl;
            EmergencyStop stoppie;

        }
    } else{
        if(emergency){
            emergency = 0;
        }
    }


}

*/

void MainWindow::emergencyStopPressed(){
    if(emergency == 0){
        emergency = 1;
        mainCamera->stopCamera();
        ardo->send(1);
        stoppie->newEmergency();


    }
}


void MainWindow::emergencyStopReleased(){
    if(waitingForRelease==0){
        if(emergency == 1){
            waitingForRelease=1;

            //wait until the emergency is over
            std::cout<<"waiting until emergency is over"<<std::endl;

            while(emergency == 1){
                usleep(100000);
                if(digitalRead(28)){
                    emergency=0;
                    std::cout<<"emergency ended"<<std::endl;
                    stoppie->emergencyGone(motorController);
                    sleep(1);
                }
            }
        }
        waitingForRelease=0;
    }
}

