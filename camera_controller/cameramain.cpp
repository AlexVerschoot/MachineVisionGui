#include "cameramain.h"
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
CameraMain::CameraMain()
{
}

CameraMain::~CameraMain()
{
}


void CameraMain::startCamera(){
    exit = 0;

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

    //a boolean to know if the program has already initialized (read: if image1 is already an image)
    bool initialized = false;
    cv::Mat imgs [2];


    cvNamedWindow("RaspiCamTest", 1);
    start = std::chrono::system_clock::now();
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
        std::thread t1(&CameraMain::comparison_thread ,this, imgs);
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

void CameraMain::stopCamera(){
    exit = 1;
}

//detect differences in the frame

void CameraMain::comparison_thread(cv::Mat ctimgs[2]) {
    //imshow("RaspiCamTest", ctimgs[1]);
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
