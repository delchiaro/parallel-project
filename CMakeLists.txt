cmake_minimum_required(VERSION 3.3)



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("debug mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")

else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("release mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")

endif (CMAKE_BUILD_TYPE STREQUAL "Debug")




set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(SOURCE_FILES src/mainSimpleStructured.cpp src/imProc/Matrix.cpp src/imProc/Image.cpp  src/imProc/point.cpp src/utils/TimeProfiler.cpp )

set(DEBUG)



project(StructuredSimple)
find_package( OpenCV REQUIRED )
add_executable(StructuredSimple src/mainSimpleStructured.cpp src/utils/TimeProfiler.cpp)
target_link_libraries( StructuredSimple ${OpenCV_LIBS}    )


project(ObjectOriented)
find_package( OpenCV HINTS "/usr/lib/x86_64-linux-gnu/" )
find_package( OpenCV REQUIRED )
add_executable( ObjectOriented src/utils/TimeProfiler.cpp src/mainOOP.cpp src/benchOOP/BenchOOP.cpp src/convolutionBench/BenchManager.cpp src/convolutionBench/IConvBench.cpp src/imProc/Image.cpp src/imProc/Matrix.cpp  src/imProc/Point.cpp  src/imProc/StructuringElement.cpp src/benchOOP/ErosionBenchOOP.cpp src/benchOOP/DilationBenchOOP.cpp src/benchOOP/OpeningBenchOOP.cpp src/benchOOP/ClosureBenchOOP.cpp )
target_link_libraries( ObjectOriented ${OpenCV_LIBS} )



project(Structured)
find_package( OpenCV REQUIRED )
add_executable(Structured src/mainStructured.cpp src/utils/TimeProfiler.cpp  src/convolutionBench/BenchManager.cpp src/convolutionBench/IConvBench.cpp  src/benchSTRUCT/BenchSTRUCT.cpp)
target_link_libraries( Structured ${OpenCV_LIBS}    )



