#ifndef MOTORCONTROLLERSTARTDIALOG_H
#define MOTORCONTROLLERSTARTDIALOG_H

#include <QDialog>
#include "SerialPort.h"

namespace Ui {
class MotorControllerStartDialog;
}

class MotorControllerStartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MotorControllerStartDialog(QWidget *parent = 0);
    ~MotorControllerStartDialog();


private:
    Ui::MotorControllerStartDialog *ui;

private slots:
    void startSerialCommuncation();
};

#endif // MOTORCONTROLLERSTARTDIALOG_H
