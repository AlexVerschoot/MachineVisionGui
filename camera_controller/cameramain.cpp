#include "cameramain.h"
#include <iostream>
#include <highgui.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <cstdio>


//TODO remove the namespaces and do everything individually
using namespace cv;
using namespace std;
CameraMain::CameraMain()
{
    std::thread t1(&CameraMain::main_camera_thread ,this, &exit, &frames);
    t1.detach();

}

CameraMain::~CameraMain()
{
}


void CameraMain::startCamera(){
    exit = 0;
    std::cout<<"exit should now be zero"<<std::endl;
}

void CameraMain::stopCamera(){
    exit = 1;
    std::cout<<"exit should now be one"<<std::endl;
}

void  CameraMain::main_camera_thread(int * exit, int * frames){
    //remove all already existing pictures from previous sessions
    std::remove("/home/pi/Pictures/motion_detected_*");

    *exit = 1;

    //TODO start the camera detection
    RASPIVID_CONFIG * config = (RASPIVID_CONFIG*) malloc(sizeof (RASPIVID_CONFIG));
    config->width = pwidth;
    config->height = pheight;
    config->bitrate = 0; // zero: leave as default
    config->framerate = 90;
    config->monochrome = 1;
    RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config);
    free(config);


    //a boolean to know if the program has already initialized (read: if image1 is already an image)
    bool initialized = false;
    cv::Mat imgs [2];


    //cvNamedWindow("RaspiCamTest", 1);
    while(1){
        while (*exit==0){
            IplImage* img = raspiCamCvQueryFrame(capture);
            cv::Mat image = cv::cvarrToMat(img);


            //cvShowImage("RaspiCamTest", img);
            if (initialized) {
                imgs[1].copyTo(imgs[0]);
                image.copyTo(imgs[1]);
            } else {
                //ensure camera is warmed up
                if (*frames > 5) {
                    initialized = true;
                    cout << "initialization done" << endl;
                }
                image.copyTo(imgs[0]);
                image.copyTo(imgs[1]);
            }

            //imshow("RaspiCamTest", imgs[1]);
            std::thread t1(&CameraMain::comparison_thread ,this, imgs);
            t1.detach();
            *frames = *frames + 1; //*frames++ did some strange things, so changed it to this. Don't know why it did strange stuff.
        }
        usleep(1000);
    }



    //cvDestroyWindow("RaspiCamTest");
    raspiCamCvReleaseCapture(&capture);
}


//detect differences in the frame

void CameraMain::comparison_thread(cv::Mat ctimgs[2]) {
    //imshow("RaspiCamTest", ctimgs[1]);
    //TODO implement a max amount of threads, to prevent lagging behind
    //initiate the variable changed_pixels and ensure the value is 0
    int changed_pixels = 0;
    //declaring this here helps more than 10 fps
    int intensity0;
    int intensity1;
    int difference;
    unsigned long amount_detected_thread;
    for (int x = 0; x < pwidth; ++x) {
        for (int y = 0; y < pheight; ++y) {
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

        //display the amount of changed pixels on the image




        cv::Mat tempMat;
        tempMat = abs(ctimgs[1] - ctimgs[0]);
        int whitePixels = 0;

        for (int x = 0; x < pwidth; ++x) {
            for (int y = 0; y < pheight; ++y) {
                //access the pixel
                intensity0 = (int) tempMat.at<uchar>(y, x);

                if (intensity0 > 25) {
                    whitePixels++;
                    tempMat.at<uchar>(y, x) = (uchar) 255;
                } else {
                    tempMat.at<uchar>(y, x) = (uchar) 0;
                }
            }
        }

        double diameter = sqrt((double(changed_pixels)/2) / 3.14159265358979323846);
        std::cout << "Motion detected " << to_string(amount_detected_thread) << std::endl;

        std::cout << "total : " << changed_pixels << "        diameter : " << diameter<< std::endl;
        putText(tempMat, "t: " + to_string(changed_pixels) + " d: " + to_string(diameter), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(200,200,250), 1, CV_AA);

        /*


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
        */
        //save the image to a picture
        cv::imwrite("/home/pi/Pictures/motion_detected_" + to_string(amount_detected_thread) + ".jpg", tempMat);



        //cv::flip(ctimgs[0], ctimgs[0], -1);
        //cv::imwrite("/home/pi/Pictures/motion_detected_" + to_string(amount_detected) +".jpg",tempMat);

        //imshow("RaspiCamTest", tempMat);
        //motorController->gotoPosition(amount_detected % 16);
    }

}


//give back how many frames have been shown
long CameraMain::getFrames(){
    return frames;
}
