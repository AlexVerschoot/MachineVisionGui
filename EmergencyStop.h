#ifndef EMERGENCYSTOP_H
#define EMERGENCYSTOP_H

#include <QDialog>
#include "motor_controller/MotorControllerSec.h"

namespace Ui {
class EmergencyStop;
}

class EmergencyStop : public QDialog
{
    Q_OBJECT

public:
    explicit EmergencyStop(QWidget *parent = 0);
    ~EmergencyStop();
    void newEmergency();
    void emergencyGone(MotorControllerSec * motorController);



private:
    Ui::EmergencyStop *ui;

    MotorControllerSec * motorController;

    int stopping = 0;


private slots:

};

#endif // MOTORCONTROLLERSTARTDIALOG_H
