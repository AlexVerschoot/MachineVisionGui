#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:34:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eindwerk
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motor_controller/MotorControllerSec.cpp \
    motor_controller/SerialPort.cpp

HEADERS  += mainwindow.h \
    motor_controller/MotorControllerSec.h \
    motor_controller/SerialPort.h

FORMS    += mainwindow.ui


target.path = /home/pi
INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    images/icon_stop.png \
    images/icon_play.png
