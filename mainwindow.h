#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
//Motor control
#include "motor_controller/MotorControllerSec.h"
#include "camera_controller/cameramain.h"
#include "I2C_controller/arduinocom.h"

#include "EmergencyStop.h"






namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    int runningTime = 0;
    void emergencyStopPressed();
    ~MainWindow();



private slots:
    void on_launcherStartButton_clicked();

    void on_launcherStopButton_clicked();

    void on_launcherSpeedControl_valueChanged(int value);

    void updateTime();

    void on_servoMotorControl_valueChanged(int value);

    void startMotor();

    void emergencyStopReleased();




private:

    QTimer timer;
    QTimer emergencyTimer;
    int emergency = 0;
    int waitingForRelease = 0;
    Ui::MainWindow *ui;

    struct last_new_ball{
        int time = 0;
        int amount = 0;
    } last_detected;

    bool running=false;

    //for the cpu info
    long double a[5][4], b[5][4], loadavg;
    //for the framerate
    long framerateOld= 0;

    const static int interval = 1000;

    //the servo
    MotorControllerSec * motorController;

    //the camera
    CameraMain * mainCamera;

    //the communication to the arduino
    ArduinoCom* ardo;

    //for the emergency stop
    EmergencyStop * stoppie;

};

#endif // MAINWINDOW_H
