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
    motor_controller/SerialPort.cpp \
    motor_controller/motorcontrollerstartdialog.cpp

HEADERS  += mainwindow.h \
    motor_controller/MotorControllerSec.h \
    motor_controller/SerialPort.h \
    motor_controller/motorcontrollerstartdialog.h

FORMS    += mainwindow.ui \
    motor_controller/motorcontrollerstartdialog.ui


target.path = /home/pi
INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    images/icon_stop.png \
    images/icon_play.png
