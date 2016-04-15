/*
 * MotorControllerSec.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: Alexv
 */

#include "MotorControllerSec.h"
#include "motorcontrollerstartdialog.h"

MotorControllerSec::MotorControllerSec() {


    MotorControllerStartDialog mcstd;

    serialPort = new SerialPort();

}

MotorControllerSec::~MotorControllerSec() {
    delete serialPort;
}

void MotorControllerSec::gotoPosition(int position) {
    //getting the int to a hex value
    std::stringstream stream;
    stream << std::hex << position;
    std::string result(stream.str());
    char c = result[0];

    if (busy == 0){
        busy = 1;
        unsigned char temp1[] = { 'S', 'P', 'O', 'S', ':', '0', c, ' ', '\r', '\n' };
        serialPort->writePort(temp1);
        usleep(10000);
        unsigned char temp2[] = "SPGO!  \r\n";
        serialPort->writePort(temp2);
        usleep(100000);
        busy = 0;
    }
}

