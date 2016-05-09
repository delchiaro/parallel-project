cmake_minimum_required(VERSION 3.3)
project(parallel_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp imProc/Matrix.cpp imProc/Matrix.h imProc/Image.cpp imProc/Image.h imProc/point.cpp imProc/point.h imProc/imProc.h)
add_executable(parallel_project ${SOURCE_FILES})




project( DisplayImage )
find_package( OpenCV REQUIRED )
add_executable( DisplayImage DisplayImage.cpp )
target_link_libraries( DisplayImage ${OpenCV_LIBS} )