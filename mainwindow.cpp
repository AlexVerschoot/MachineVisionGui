#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <highgui.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <string.h>
#include <memory.h>

//TODO remove the namespaces and do everything individually
using namespace cv;
using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //a timer that counts every second
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer.setInterval(1000);
    timer.start();
    QTimer::singleShot(1, this, SLOT(startMotor()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_launcherStartButton_clicked()
{


    //TODO include some serial communication with the arduino
    std::cout<<"launcher should now start"<<std::endl;

    //TODO start the camera detection
    RASPIVID_CONFIG * config = (RASPIVID_CONFIG*) malloc(sizeof (RASPIVID_CONFIG));
    config->width = 320;
    config->height = 240;
    config->bitrate = 0; // zero: leave as default
    config->framerate = 90;
    config->monochrome = 1;
    RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config);
    free(config);

    std::chrono::time_point<std::chrono::system_clock> start, end;


    cvNamedWindow("RaspiCamTest", 1);
    start = std::chrono::system_clock::now();
    int exit = 0;
    do {
        IplImage* img = raspiCamCvQueryFrame(capture);
        cv::Mat image = cv::cvarrToMat(img);


        //cvShowImage("RaspiCamTest", img);
        if (initialized) {
            imgs[1].copyTo(imgs[0]);
            image.copyTo(imgs[1]);
        } else {
            //ensure camera is warmed up
            if (frames > 5) {
                initialized = true;
                cout << "initialization done" << endl;
            }
            image.copyTo(imgs[0]);
            image.copyTo(imgs[1]);
        }

        //imshow("RaspiCamTest", imgs[1]);
        std::thread t1(&MainWindow::comparison_thread ,this, imgs);
        t1.detach();
        frames++;

        char key = cvWaitKey(2);

        //ESC to exit
        if (key == 27) {
            exit = 1;
        }


    } while (!exit);

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    cout << "shown frames : " << frames << " | time : " << elapsed_seconds.count() << "s | fps : " << frames / elapsed_seconds.count() << endl;
    cvDestroyWindow("RaspiCamTest");
    raspiCamCvReleaseCapture(&capture);
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

    //TODO stop the camera detection

}

void MainWindow::updateTime()
{
    runningTime ++;
    //the arg is number, amount of numbers, decimal, what number
    //this updates the status time
    QString number = QString("%1:").arg((runningTime/3600), 2, 10, QChar('0')) + QString("%1:").arg((runningTime/60%60), 2, 10, QChar('0'))+QString("%1").arg(runningTime%60, 2, 10, QChar('0'));
    ui->label_activeTime->setText(number);


    //the cpu activity
    //TODO put this in a library, improve the code so i can better reuse it in future programs
    char buf[100];
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 4; ++y) {
            a[x][y]=b[x][y];
        }
    }


    FILE * pFile2 = fopen ("/proc/stat" , "r");
    for (int var = 0; var < 5; ++var) {
        if (fgets(buf, sizeof buf, pFile2) != NULL){
            sscanf(buf,"%*s %Lf %Lf %Lf %Lf",&b[var][0],&b[var][1],&b[var][2],&b[var][3]);
        }
    }
    fclose(pFile2);
    for (int var = 0; var < 5; ++var) {
        loadavg = ((b[var][0]+b[var][1]+b[var][2]) - (a[var][0]+a[var][1]+a[var][2])) / ((b[var][0]+b[var][1]+b[var][2]+b[var][3]) - (a[var][0]+a[var][1]+a[var][2]+a[var][3]));

        switch (var) {
        case 1:
            ui->progress_cpu0->setValue(loadavg*100);
            break;
        case 2:
            ui->progress_cpu1->setValue(loadavg*100);
            break;
        case 3:
            ui->progress_cpu2->setValue(loadavg*100);
            break;
        case 4:
            ui->progress_cpu3->setValue(loadavg*100);
            break;
        default:
            ui->progress_cpuTotal->setValue(loadavg*100);
            break;
        }
    }

    //the temperature
    int tempe;
    FILE * pFile3 = fopen ("/sys/class/thermal/thermal_zone0/temp" , "r");
    if (fgets(buf, sizeof buf, pFile3) != NULL){
        sscanf(buf,"%i",&tempe);
        QString number ;//= number.number(tempe/1000);
        ui->label_temperature->setText(number.number(tempe/1000)+" °C");
    }else{
        std::cout<<"could not read temp"<<std::endl;
    }
    fclose(pFile3);

}


void MainWindow::on_servoMotorControl_valueChanged(int value)
{
    motorController->gotoPosition(value);
}

void MainWindow::startMotor(){
    //TODO make a settings menu where this can be redone
    motorController = new MotorControllerSec();
}

//detect differences in the frame

void MainWindow::comparison_thread(cv::Mat ctimgs[2]) {
    imshow("RaspiCamTest", ctimgs[1]);
    /*
    //TODO implement a max amount of threads, to prevent lagging behind
    //initiate the variable changed_pixels and ensure the value is 0
    int changed_pixels = 0;
    //declaring this here helps more than 10 fps
    int intensity0;
    int intensity1;
    int difference;
    unsigned long amount_detected_thread;
    for (int x = 0; x < 320; ++x) {
        for (int y = 0; y < 240; ++y) {
            //access the pixel
            intensity0 = (int) ctimgs[0].at<uchar>(y, x);
            intensity1 = (int) ctimgs[1].at<uchar>(y, x);
            //only check the green value
            //the difference
            difference = abs(intensity1 - intensity0);


            if (difference > THRESHOLD) {
                changed_pixels++;
            }
        }
    }

    if (changed_pixels > SENSITIVITY) {
        amount_detected++;
        amount_detected_thread = amount_detected;
        std::cout << "Motion detected " << to_string(amount_detected_thread) << std::endl;

        cv::Mat tempMat;
        tempMat = abs(ctimgs[1] - ctimgs[0]);

        for (int x = 0; x < 320; ++x) {
            for (int y = 0; y < 240; ++y) {
                //access the pixel
                intensity0 = (int) tempMat.at<uchar>(y, x);

                if (intensity0 > 25) {
                    tempMat.at<uchar>(y, x) = (uchar) 255;
                } else {
                    tempMat.at<uchar>(y, x) = (uchar) 0;
                }
            }
        }

        // smooth it, otherwise a lot of false circles may be detected
        GaussianBlur(tempMat, tempMat, Size(9, 9), 2, 2);
        vector<Vec3f> circles;
        HoughCircles(tempMat, circles, CV_HOUGH_GRADIENT, 1, tempMat.rows / 4, 5, 25, 5, 0);
        cvtColor(tempMat, tempMat, CV_GRAY2RGB);

        for (size_t i = 0; i < circles.size(); i++) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // draw the circle center
            circle(tempMat, center, 3, Scalar(0, 255, 0), -1, 8, 0);
            // draw the circle outline
            circle(tempMat, center, radius, Scalar(0, 0, 255), 3, 8, 0);
            //display the shown radius
            putText(tempMat, "radius : " + to_string(radius), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(200,200,250), 1, CV_AA);
        }

        //save the image to a picture
        //cv::imwrite("/home/pi/Pictures/motion_detected_" + to_string(amount_detected_thread) + ".jpg", tempMat);



        //cv::flip(ctimgs[0], ctimgs[0], -1);
        //cv::imwrite("/root/Pictures/motion_detected_" + to_string(amount_detected) +".jpg",tempMat);

        //imshow("RaspiCamTest", tempMat);
        //motorController->gotoPosition(amount_detected % 16);
    }
    */
}

