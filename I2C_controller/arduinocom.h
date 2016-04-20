#ifndef ARDUINOCOM_H
#define ARDUINOCOM_H

//http://www.raspberry-projects.com/pi/programming-in-c/i2c/using-the-i2c-interface


class ArduinoCom
{
public:
    ArduinoCom();
    ~ArduinoCom();

    void send(int command);
    void send(int command, int percentage);

private :
    int file_i2c;

};

#endif // ARDUINOCOM_H
