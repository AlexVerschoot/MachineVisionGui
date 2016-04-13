#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
//Motor control
#include "motor_controller/MotorControllerSec.h"
//camera control
#include "camera_controller/RaspiCamCV.h"
#include <opencv/cv.h>
#include <thread>



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

    void comparison_thread(cv::Mat ctimgs[2]);


private:

    QTimer timer;
    Ui::MainWindow *ui;

    //for the cpu info
    long double a[5][4], b[5][4], loadavg;

    //the servo
    MotorControllerSec * motorController;

    //motion detection values
    //the minimum value to consider the pixel as different
    const int THRESHOLD = 30;
    //the amount of pixels that need to be different
    const int SENSITIVITY = 10;
    //remembers the total amount of detected motions
    unsigned long amount_detected = 0;

    //a boolean to know if the program has already initialized (read: if image1 is already an image)
    bool initialized = false;
    cv::Mat imgs [2];
    long frames = 0;

};

#endif // MAINWINDOW_H
