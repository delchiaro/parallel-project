cmake_minimum_required(VERSION 3.3)
project(parallel_project)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fopenmp -I/opt/local/include/libomp/ -L/opt/local/lib/libomp/")

set(SOURCE_FILES DisplayImage.cpp)

add_executable(parallel_project ${SOURCE_FILES})


find_package( OpenCV REQUIRED )
target_link_libraries( parallel_project ${OpenCV_LIBS} )