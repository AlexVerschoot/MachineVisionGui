/*
 * MotorControllerSec.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: Alexv
 */

#include "MotorControllerSec.h"

MotorControllerSec::MotorControllerSec() {


    //MotorControllerStartDialog mcstd;

    serialPort = new SerialPort();

    initialize();

}

MotorControllerSec::~MotorControllerSec() {
    delete serialPort;
}

void MotorControllerSec::initialize(){
    //write drive enable
    sleep(1);
    unsigned char temp[] = "SRF+ \r\n";
    serialPort->writePort(temp);
    sleep(1);
    gotoPosition(0);
}

void MotorControllerSec::gotoPosition(int position) {
    //getting the int to a hex value
    std::stringstream stream;
    stream << std::hex << position;
    std::string result(stream.str());
    char c = result[0];

//ensure we only send one position at the time, and don't have intterupted messages
    if (busy == 0){
        busy = 1;
        unsigned char temp1[] = { 'S', 'P', 'O', 'S', ':', '0', c, ' ', '\r', '\n' };
        serialPort->writePort(temp1);
        //wait 10 ms to give the motorcontroller enough time to process this
        usleep(10000);
        unsigned char temp2[] = "SPGO!  \r\n";
        serialPort->writePort(temp2);
        //wait 10 ms to give the motorcontroller enough time to process this
        usleep(100000);
        //indicate that we can go to a new position
        busy = 0;
    }
}

