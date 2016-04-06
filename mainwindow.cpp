#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //a timer that counts every second
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer.setInterval(1000);
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_launcherStartButton_clicked()
{
    //TODO include some serial communication with the arduino
    std::cout<<"launcher should now start"<<std::endl;
}

void MainWindow::on_launcherStopButton_clicked()
{
    //TODO include some serial communication with the arduino
    std::cout<<"launcher should now stop"<<std::endl;
}

void MainWindow::on_launcherSpeedControl_valueChanged(int value)
{
    //TODO include some serial communication with the arduino
    std::cout<<"speed in % : " <<value<<std::endl;

}

void MainWindow::updateTime()
{
    runningTime = runningTime + 100;
    //std::cout << "time active :" << runningTime << std::endl;
    //the arg is number, amount of numbers, decimal, what number
    //this
    QString number = QString("%1:").arg((runningTime/3600), 2, 10, QChar('0')) + QString("%1:").arg((runningTime/60%60), 2, 10, QChar('0'))+QString("%1").arg(runningTime%60, 2, 10, QChar('0'));
    ui->label_activeTime->setText(number);
}
