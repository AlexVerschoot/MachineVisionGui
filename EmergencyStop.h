#ifndef EMERGENCYSTOP_H
#define EMERGENCYSTOP_H

#include <QDialog>

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
    void emergencyGone();



private:
    Ui::EmergencyStop *ui;

private slots:

};

#endif // MOTORCONTROLLERSTARTDIALOG_H
