#ifndef CAMERAMAIN_H
#define CAMERAMAIN_H

//camera control
#include "RaspiCamCV.h"
#include <opencv/cv.h>
#include <thread>


class CameraMain
{
public:
    CameraMain();
    ~CameraMain();
    void stopCamera();
    void startCamera();
    long getFrames();


private:
    void comparison_thread(cv::Mat ctimgs[2]);

    void main_camera_thread(int * exit, int * frames);


    //motion detection values
    //the minimum value to consider the pixel as different
    const int THRESHOLD = 30;
    //the amount of pixels that need to be different
    const int SENSITIVITY = 10;
    //remembers the total amount of detected motions
    unsigned long amount_detected = 0;

    //widht and height of the frames
    const static int pwidth = 320;
    const static int pheight = 240;


    int frames = 0;

    //an int to stop
    int exit = 0;

};

#endif // CAMERAMAIN_H
