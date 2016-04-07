/*
 * MotorControllerSec.cpp
 *
 *  Created on: Mar 1, 2016
 *      Author: Alexv
 */

#include "MotorControllerSec.h"

MotorControllerSec::MotorControllerSec() {
	serialPort = new SerialPort();

	std::cout << "Please turn on the motor controller." << std::endl;
	std::cout
			<< "If the motor controller is already turned on, please restart it."
			<< std::endl;

	//init for when you start the controller
	for (int var = 0; var < 15; ++var) {
		serialPort->readPort();
	}

	//write drive enable
	sleep(1);
	unsigned char temp[] = "SRF+ \r\n";
	std::cout << "drive enabled" << std::endl;
	serialPort->writePort(temp);
	sleep(1);

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

	unsigned char temp1[] = { 'S', 'P', 'O', 'S', ':', '0', c, ' ', '\r', '\n' };
	serialPort->writePort(temp1);
	usleep(10000);
	unsigned char temp2[] = "SPGO!  \r\n";
	serialPort->writePort(temp2);
	usleep(100000);
}

