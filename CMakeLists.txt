cmake_minimum_required(VERSION 2.8)
project(demo)

find_package(OpenCV REQUIRED)

include_directories(
	include
	/usr/include/opencv
	/usr/include/boost/thread
)

add_executable(demo demo.cpp src/EventStream.cpp src/FrameStream.cpp)

target_link_libraries(demo pthread ${OpenCV_LIBS})