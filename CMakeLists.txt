cmake_minimum_required(VERSION 3.0.2)
project(MyProject)

#OPENCV
find_package( OpenCV REQUIRED )

#QT
find_package(Qt5Widgets)
find_package(Qt5LinguistTools)

SET(CMAKE_EXE_LINKER_FLAGS "-lpthread -lwiringPi")
include_directories(/opt/vc/include)
include_directories(/opt/vc/include/EGL)
include_directories(/opt/vc/userland/host_applications/linux/libs/bcm_host/include)
include_directories(/opt/vc/userland/interface/vcos)
include_directories(/opt/vc/userland)
include_directories(/opt/vc/userland/interface/vcos/pthreads)
include_directories(/opt/vc/userland/interface/vmcs_host/linux)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_PATH)

find_package(Qt5Core REQUIRED)

qt5_add_resources(QT_RESOURCE qtimages.qrc)
#QT5_CREATE_TRANSLATION(languages/Translation_en.ts  languages/Translation_nl.ts)
#qt5_add_translation(languages/Translation_en.ts  languages/Translation_nl.ts)

# This allows files that are not compiled to be added to a project
add_custom_target(MyProjectSources SOURCES
mainwindow.ui
EmergencyStop.ui
timeOut.ui
)


add_library(mainwindow mainwindow.cpp )
target_link_libraries (mainwindow Qt5::Widgets)


add_executable(MyProject main.cpp motor_controller/MotorControllerSec.cpp motor_controller/SerialPort.cpp camera_controller/RaspiCamCV.c camera_controller/RaspiCLI.c camera_controller/RaspiCamControl.c camera_controller/cameramain I2C_controller/arduinocom.cpp EmergencyStop.cpp timeOut.cpp ${QT_RESOURCE})
target_link_libraries (MyProject mainwindow)
target_link_libraries(MyProject m /opt/vc/lib/libmmal_core.so /opt/vc/lib/libmmal_util.so /opt/vc/lib/libmmal_vc_client.so /opt/vc/lib/libvcos.so /opt/vc/lib/libbcm_host.so /opt/vc/lib/libGLESv2.so /opt/vc/lib/libEGL.so  ${OpenCV_LIBS})


#ensure c++ 11 for the presence off threads
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
