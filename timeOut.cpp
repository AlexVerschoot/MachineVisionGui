#include "timeOut.h"
#include "ui_timeOut.h"
#include "QTimer"
#include "unistd.h"
#include "iostream"



TimeOut::TimeOut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeOut)
{
    ui->setupUi(this);
    //QTimer::singleShot(100, this, SLOT(startSerialCommuncation()));
    exec();
}

TimeOut::~TimeOut()
{
    delete ui;
}
