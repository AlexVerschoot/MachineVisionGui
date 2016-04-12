cmake_minimum_required(VERSION 3.0.2)
project(MyProject)

find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

qt5_add_resources(QT_RESOURCE qtimages.qrc)
qt5_add_resources(QT_FORM mainwindow.ui)


add_library(mainwindow mainwindow.cpp )
target_link_libraries (mainwindow Qt5::Widgets)


add_executable(MyProject main.cpp motor_controller/MotorControllerSec.cpp motor_controller/SerialPort.cpp motor_controller/motorcontrollerstartdialog.cpp ${QT_RESOURCE})
target_link_libraries (MyProject mainwindow)

