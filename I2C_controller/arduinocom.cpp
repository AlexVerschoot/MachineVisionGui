#include "arduinocom.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstring>

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

ArduinoCom::ArduinoCom()
{
    //----- OPEN THE I2C BUS -----
    char *filename = (char*)"/dev/i2c-1";
    if ((file_i2c = open(filename, O_RDWR)) < 0)
    {
        //ERROR HANDLING: you can check errno to see what went wrong
        std::cout<<"Failed to open the i2c bus"<<std::endl;
        return;
    }

    int addr = 0x04;          //<<<<<The I2C address of the slave
    if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
    {
        std::cout<<"Failed to acquire bus access and/or talk to slave.\n"<<std::endl;
        //ERROR HANDLING; you can check errno to see what went wrong
        return;
    }

}

ArduinoCom::~ArduinoCom(){
}

void ArduinoCom::send(int command){
    char buffer[64];
    switch (command)
    {
    case 0:
        //copy the string into buffer
        std::strcpy(buffer, "B"); //B from begin
        break;
    case 1:
        std::strcpy(buffer, "H"); //H from halt
        break;
    case 3:
        std::strcpy(buffer, "C"); //C from confirmed
        break;
    default:
        std::cout << "wrong command"<<std::endl;
        break;
    }
    //----- WRITE BYTES -----
    if (write(file_i2c, buffer, std::strlen(buffer)) != std::strlen(buffer))		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        std::cout<<"Failed to write to the i2c bus.\n"<<std::endl;
    }
}

void ArduinoCom::send(int command, int percentage){
    char buffer[64];
    switch (command)
    {
    case 0:
        //copy the string into buffer
        std::strcpy(buffer, "B"); //B from begin
        break;
    case 1:
        std::strcpy(buffer, "H"); //H from halt
        break;
    case 2:
        //copy the string into buffer
        std::strcpy(buffer, (std::to_string(percentage)).c_str()); //S from speed + the percentage
        break;
    case 3:
        std::strcpy(buffer, "C"); //C from confirmed
        break;
    default:
        std::cout << "wrong command"<<std::endl;
        break;
    }
    //----- WRITE BYTES -----
    if (write(file_i2c, buffer, std::strlen(buffer)) != std::strlen(buffer))		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
    {
        /* ERROR HANDLING: i2c transaction failed */
        std::cout<<"Failed to write to the i2c bus.\n"<<std::endl;
    }
}

