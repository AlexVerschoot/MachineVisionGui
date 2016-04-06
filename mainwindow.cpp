#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
