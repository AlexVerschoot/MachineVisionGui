/*
 * SerialPort.h
 *
 *  Created on: Feb 29, 2016
 *      Author: Alexv
 */

#ifndef SERIALPORT_H_
#define SERIALPORT_H_

#include <iostream>
#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions

// http://stackoverflow.com/questions/18108932/linux-c-serial-port-reading-writing

class SerialPort {
private :
	int USB;
	struct termios tty;
	struct termios tty_old;
public:
	SerialPort();
	virtual ~SerialPort();
	void readPort();
	void writePort(unsigned char cmd[]);
};

#endif /* SERIALPORT_H_ */
