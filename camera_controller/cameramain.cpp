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
CameraMain::CameraMain(MotorControllerSec * motorController)
{
    motorController->gotoPosition(8);


    std::thread t1(&CameraMain::main_camera_thread ,this, &exit, &frames, motorController);
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

void  CameraMain::main_camera_thread(int * exit, int * frames, MotorControllerSec * motorController){
    int done = 0;
    //remove all already existing pictures from previous sessions
    for (int var = 1; done == 0; var++) {
        //TODO put the save location in a more central place"
        std::string s = "/home/pi/Pictures/motion_detected_" + to_string(var) + ".jpg";
        //cout << s << endl;
        const char * filename = s.c_str();
        if(std::remove(filename) != 0){
            done = 1;
        }
        ;
    }


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


    //a boolean to know if the program has already initialized
    bool initialized = false;

    //cvNamedWindow("RaspiCamTest", 1);
    while(1){
        while (*exit==0){
            IplImage* img = raspiCamCvQueryFrame(capture);
            cv::Mat image = cv::cvarrToMat(img);


            //cvShowImage("RaspiCamTest", img);
            if (initialized) {
                //imshow("RaspiCamTest", imgs[1]);
                std::thread t1(&CameraMain::comparison_thread ,this, image, motorController);
                t1.detach();
            } else {
                //ensure camera is warmed up
                if (*frames > 5) {
                    initialized = true;
                    cout << "initialization done" << endl;
                }

            }
            *frames = *frames + 1; //*frames++ did some strange things, so changed it to this. Don't know why it did strange stuff.


        }
        usleep(1000);
    }



    //cvDestroyWindow("RaspiCamTest");
    raspiCamCvReleaseCapture(&capture);
}


//detect differences in the frame

void CameraMain::comparison_thread(cv::Mat ctimgs, MotorControllerSec * motorController) {
    int current_frame = frames;
    //imshow("RaspiCamTest", ctimgs[1]);
    //TODO implement a max amount of threads, to prevent lagging behind
    //initiate the variable changed_pixels and ensure the value is 0
    int changed_pixels = 0;
    //declaring this here helps more than 10 fps
    int intensity;
    int difference;
    unsigned long amount_detected_thread;




    for (int x = 0; x < pwidth; ++x) {
        for (int y = 0; y < pheight; ++y) {
            //access the pixel
            intensity = (int) ctimgs.at<uchar>(y, x);

            if (intensity < THRESHOLD) {
                changed_pixels++;
            }
        }
    }



    if (changed_pixels > SENSITIVITY) {
        amount_detected++;
        amount_detected_thread = amount_detected;


        //display the amount of changed pixels on the image
        //std::cout << "black pixels : " << changed_pixels << std::endl;

        cv::Mat tempMat;
        ctimgs.copyTo(tempMat);
        //tempMat = abs(ctimgs[1] - ctimgs[0]);
        int whitePixels = 0;


        for (int x = 0; x < pwidth; ++x) {
            for (int y = 0; y < pheight; ++y) {
                //access the pixel
                intensity = (int) tempMat.at<uchar>(y, x);
                //an int to see how mutch pixels are in close proximity
                int siblingPoints=0;
                if (y>0){
                    siblingPoints += (int) tempMat.at<uchar>(y-1, x);
                }
                if(x>0){
                    siblingPoints += (int) tempMat.at<uchar>(y, x-1);

                }
                if(y<pheight){
                    siblingPoints += (int) tempMat.at<uchar>(y+1, x);
                }

                if(x<pwidth){
                    siblingPoints += (int) tempMat.at<uchar>(y, x-1);
                }

                //if the intensity is bigger than the min required, and atleast two pixels next to it is also activated, make it white.
                if (intensity < THRESHOLD && siblingPoints < THRESHOLD*2) {
                    whitePixels++;
                    tempMat.at<uchar>(y, x) = (uchar) 255;
                } else {
                    tempMat.at<uchar>(y, x) = (uchar) 0;
                }
            }
        }

        // smooth it, otherwise a lot of false circles may be detected
        /*GaussianBlur(tempMat, tempMat, Size(5, 5), 2, 2);

        for (int x = 0; x < pwidth; ++x) {
            for (int y = 0; y < pheight; ++y) {
                //access the pixel
                intensity = (int) tempMat.at<uchar>(y, x);

                if (intensity > THRESHOLD) {
                    tempMat.at<uchar>(y, x) = (uchar) 255;
                    whitePixels++;
                } else {
                    tempMat.at<uchar>(y, x) = (uchar) 0;
                }
            }
        }
        */

        //say that i'm done with the heavy stuff
        while (amount_detected_thread-1 != finished_detected){
            usleep(100);
        }
        finished_detected = amount_detected_thread;
        //std::cout << last_detected.frames << "                  " << current_frame << std::endl;

        if(last_detected.frames  == current_frame -1){
            last_detected.frames = current_frame;
            if(last_detected.whites<whitePixels){
                last_detected.whites = whitePixels;
                last_detected.motiondetected = amount_detected_thread;


                std::stringstream msg;
                msg << "CompleteMotion detected " << to_string(amount_detected_thread) << "        total : " << whitePixels << "           current frame:" <<current_frame << std::endl;
                std::cout << msg.str();


                putText(tempMat, " total: " + to_string(whitePixels), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(200,200,250), 1, CV_AA);

                //save the image to a picture
                cv::imwrite("/home/pi/Pictures/motion_detected_" + to_string(amount_detected_thread) + ".jpg", tempMat);
                //motorController->gotoPosition(amount_detected_thread%16);
                //cout << "motor now moving to position" << amount_detected_thread %16 << endl;


                //cv::flip(ctimgs[0], ctimgs[0], -1);

                //imshow("RaspiCamTest", tempMat);
                //motorController->gotoPosition(amount_detected % 16);
            }
        } else {
            last_detected.frames = current_frame;
            last_detected.done = false;
            last_detected.motiondetected = amount_detected_thread;


            std::stringstream msg;
            msg << "FirstMotion detected " << to_string(amount_detected_thread) << "        total : " << whitePixels << "           current frame:" <<current_frame << std::endl;
            std::cout << msg.str();


            putText(tempMat, " total: " + to_string(whitePixels), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 1, cvScalar(200,200,250), 1, CV_AA);

            //save the image to a picture
            cv::imwrite("/home/pi/Pictures/motion_detected_" + to_string(amount_detected_thread) + ".jpg", tempMat);
            //motorController->gotoPosition(amount_detected_thread%16);
            //cout << "motor now moving to position" << amount_detected_thread %16 << endl;


            //cv::flip(ctimgs[0], ctimgs[0], -1);

            //imshow("RaspiCamTest", tempMat);
            //motorController->gotoPosition(amount_detected % 16);
        }



    } else {
        if (last_detected.done == false){
            last_detected.done = true;

        }
    }


}


//give back how many frames have been shown
long CameraMain::getFrames(){
    return frames;
}

int CameraMain::getAmountDetected(){
    return amount_detected;
}

int CameraMain::getLastDetected(){
    return last_detected.motiondetected;
}

int CameraMain::getSize(){

}
