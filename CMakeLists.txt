cmake_minimum_required(VERSION 3.3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")


set(SOURCE_FILES Structured.cpp imProc/Matrix.cpp imProc/Matrix.h imProc/Image.cpp imProc/Image.h imProc/point.cpp imProc/point.h imProc/imProc.h TimeProfiler.cpp TimeProfiler.h)




project(ObjectOriented)
find_package( OpenCV REQUIRED )
add_executable( ObjectOriented DisplayImage.cpp)
target_link_libraries( ObjectOriented ${OpenCV_LIBS} )



project(Structured)
find_package( OpenCV REQUIRED )
add_executable(Structured Structured.cpp)
target_link_libraries( Structured ${OpenCV_LIBS} )
