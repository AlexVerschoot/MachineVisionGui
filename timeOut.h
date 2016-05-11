#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <QDialog>
#include "motor_controller/MotorControllerSec.h"

namespace Ui {
class TimeOut;
}

class TimeOut : public QDialog
{
    Q_OBJECT

public:
    explicit TimeOut(QWidget *parent = 0);
    ~TimeOut();




private:
    Ui::TimeOut *ui;




private slots:

};

#endif // MOTORCONTROLLERSTARTDIALOG_H
