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
    ~MainWindow();

private slots:
    void on_launcherStartButton_clicked();

    void on_launcherStopButton_clicked();

    void on_launcherSpeedControl_valueChanged(int value);

    void updateTime();

    void on_servoMotorControl_valueChanged(int value);

    void startMotor();

    //static void emergencyStopPressed();
    //static void emergencyStopReleased();

private:

    QTimer timer;
    QTimer emergencyTimer;
    int emergency = 0;
    Ui::MainWindow *ui;


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

};

#endif // MAINWINDOW_H
