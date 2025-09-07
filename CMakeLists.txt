cmake_minimum_required(VERSION 3.5)
project( DisplayImage )

find_package( OpenCV REQUIRED )
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( ObjectDetection ObjectDetection.cpp )
target_link_libraries( ObjectDetection ${OpenCV_LIBS} Eigen3::Eigen)