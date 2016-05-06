/*
 * MotorControllerSec.h
 *
 *  Created on: Mar 1, 2016
 *      Author: Alexv
 */

#ifndef MOTORCONTROLLERSEC_H_
#define MOTORCONTROLLERSEC_H_

#include <iostream>
#include "SerialPort.h"
#include "unistd.h"
#include <sstream>

class MotorControllerSec {
private:
	SerialPort * serialPort;

    int busy = 0;
public:
	MotorControllerSec();
	virtual ~MotorControllerSec();
	/* position in steps of 22.5° or 0.0625 rotations.

	 */
	void gotoPosition(int position);
    void initialize();
};

#endif /* MOTORCONTROLLERSEC_H_ */
