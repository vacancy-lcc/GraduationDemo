cmake_minimum_required(VERSION 3.0)
project(licc_cap_rfid)

# 设置交叉编译工具链
SET(CMAKE_C_COMPILER /usr/local/arm/5.4.0/usr/bin/arm-none-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/arm/5.4.0/usr/bin/arm-none-linux-gnueabi-g++)

# 设置 C++ 11 标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc、uic、rcc automatically when needed.
# 全局控制，打开全区moc...
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(RESOURCE_DIR resources/resources.qrc)


# QT
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /home/gec/tools/Qt-Embedded/Qt-Embedded-5.7.0/lib/cmake)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED) 
find_package(Qt5Test REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)

link_libraries(Qt5::Concurrent Qt5::Core Qt5::Gui Qt5::OpenGL Qt5::Test Qt5::Widgets
                Qt5::Sql Qt5::Network)

get_target_property(QtCore_location Qt5::Core LOCATION)
message(${QtCore_location})
if(Qt5Widgets_FOUND)
    message("Qt5Widgets_LIBRARIES : ${Qt5Widgets_LIBRARIES}")
    message("Qt5Widgets_INCLUDE_DIRS : ${Qt5Widgets_INCLUDE_DIRS}")
endif(Qt5Widgets_FOUND)

# arm opencv 3.4.2
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /home/gec/software/arm-opencv-3.4.2/share)
find_package(OpenCV 3.4.2 REQUIRED)
link_libraries(${OpenCV})

# jpeg
set(JPEG_PREFIX /home/gec/tools/jpglib)
include_directories(${JPEG_PREFIX}/include)
link_directories(${JPEG_PREFIX}/lib)


file(GLOB SOURCES_FILE "*.c" 
                        "*.cpp")
file(GLOB HEADER_FILE "*.h" "*.hpp")

add_executable(GraduationDemo ${SOURCES_FILE} ${HEADER_FILE})

target_link_libraries(GraduationDemo pthread jpeg )

# 设置安装路径
set(CMAKE_INSTALL_PREFIX /a)
install(TARGETS GraduationDemo DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)