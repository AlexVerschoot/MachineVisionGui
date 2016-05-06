#ifndef CAMERAMAIN_H
#define CAMERAMAIN_H

//camera control
#include "motor_controller/MotorControllerSec.h"
#include "RaspiCamCV.h"
#include <opencv/cv.h>
#include <thread>


class CameraMain
{
public:
    CameraMain(MotorControllerSec * motorController);
    ~CameraMain();
    void stopCamera();
    void startCamera();
    long getFrames();
    int getAmountDetected();
    int getLastDetected();


private:
    void comparison_thread(cv::Mat ctimgs, MotorControllerSec * motorController);

    void main_camera_thread(int * exit, int * frames, MotorControllerSec * motorController);

    int getSize(int whitePixels);


    //motion detection values
    //the minimum value to consider the pixel as different
    const int THRESHOLD = 2;
    //the amount of pixels that need to be different
    const int SENSITIVITY = 100; //10 with a resolution of 320*240
    //remembers the total amount of detected motions
    unsigned long amount_detected = 0;

    //widht and height of the frames
    const static int pwidth = 480; //320
    const static int pheight = 360; //240


    int frames = 0;
    int finished_detected = 0;

    struct motionFrame{
        int frames;
        int whites;
        int motiondetected;
        bool done;
    }last_detected;

    const static int smallMax = 2350;
    const static int bigMin = 2750;

    int small = 0;
    int medium = 0;
    int big = 0;



    //an int to stop
    int exit = 0;


};

#endif // CAMERAMAIN_H
