cmake_minimum_required(VERSION 3.0.2)
project(MyProject)

#OPENCV
find_package( OpenCV REQUIRED )

#QT
find_package(Qt5Widgets)

SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")
include_directories(/opt/vc/include)
include_directories(/opt/vc/include/EGL)
include_directories(/opt/vc/userland/host_applications/linux/libs/bcm_host/include)
include_directories(/opt/vc/userland/interface/vcos)
include_directories(/opt/vc/userland)
include_directories(/opt/vc/userland/interface/vcos/pthreads)
include_directories(/opt/vc/userland/interface/vmcs_host/linux)
include_directories(/home/pi/camcv)
include_directories(/home/pi/camcv/camera_control)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_PATH)

find_package(Qt5Core REQUIRED)

qt5_add_resources(QT_RESOURCE qtimages.qrc)

# This allows files that are not compiled to be added to a project
add_custom_target(MyProjectSources SOURCES
mainwindow.ui
motor_controller/motorcontrollerstartdialog.ui
)


add_library(mainwindow mainwindow.cpp )
target_link_libraries (mainwindow Qt5::Widgets)


add_executable(MyProject main.cpp motor_controller/MotorControllerSec.cpp motor_controller/SerialPort.cpp motor_controller/motorcontrollerstartdialog.cpp camera_controller/RaspiCamCV.c camera_controller/RaspiCLI.c camera_controller/RaspiCamControl.c ${QT_RESOURCE})
target_link_libraries (MyProject mainwindow)
target_link_libraries(MyProject m /opt/vc/lib/libmmal_core.so /opt/vc/lib/libmmal_util.so /opt/vc/lib/libmmal_vc_client.so /opt/vc/lib/libvcos.so /opt/vc/lib/libbcm_host.so /opt/vc/lib/libGLESv2.so /opt/vc/lib/libEGL.so  ${OpenCV_LIBS})


