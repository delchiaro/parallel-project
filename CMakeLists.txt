cmake_minimum_required(VERSION 3.3)

 set(OPT_FLAG "-O0")
# set(OPT_FLAG "-O1")
# set(OPT_FLAG "-O1")
# set(OPT_FLAG "-O2")

# add_definitions( -DVERBOSE_INLINE_STATUS ) # it will define VERBOSE_INLINE_STATUS to show output about the inline status.


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("debug mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug${OPT_FLAG}")

else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("release mode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/release${OPT_FLAG}")

endif (CMAKE_BUILD_TYPE STREQUAL "Debug")




set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPT_FLAG} -std=c++11 -fopenmp")
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









project(ObjectOriented-forceInline)
find_package( OpenCV HINTS "/usr/lib/x86_64-linux-gnu/" )
find_package( OpenCV REQUIRED )
add_executable( ObjectOriented-forceInline src/utils/TimeProfiler.cpp src/mainOOP.cpp src/benchOOP/BenchOOP.cpp src/convolutionBench/BenchManager.cpp src/convolutionBench/IConvBench.cpp src/imProc/Image.cpp src/imProc/Matrix.cpp  src/imProc/Point.cpp  src/imProc/StructuringElement.cpp src/benchOOP/ErosionBenchOOP.cpp src/benchOOP/DilationBenchOOP.cpp src/benchOOP/OpeningBenchOOP.cpp src/benchOOP/ClosureBenchOOP.cpp )
target_link_libraries( ObjectOriented-forceInline ${OpenCV_LIBS} )

project(ObjectOriented-forceInlineDisabled)
find_package( OpenCV HINTS "/usr/lib/x86_64-linux-gnu/" )
find_package( OpenCV REQUIRED )
add_executable( ObjectOriented-forceInlineDisabled src/utils/TimeProfiler.cpp src/mainOOP.cpp src/benchOOP/BenchOOP.cpp src/convolutionBench/BenchManager.cpp src/convolutionBench/IConvBench.cpp src/imProc/Image.cpp src/imProc/Matrix.cpp  src/imProc/Point.cpp  src/imProc/StructuringElement.cpp src/benchOOP/ErosionBenchOOP.cpp src/benchOOP/DilationBenchOOP.cpp src/benchOOP/OpeningBenchOOP.cpp src/benchOOP/ClosureBenchOOP.cpp )
target_link_libraries( ObjectOriented-forceInlineDisabled ${OpenCV_LIBS} )
set_target_properties(ObjectOriented-forceInlineDisabled PROPERTIES COMPILE_DEFINITIONS "DISABLE_FORCE_INLINE")


project(ObjectOriented-inlineDisabled)
find_package( OpenCV HINTS "/usr/lib/x86_64-linux-gnu/" )
find_package( OpenCV REQUIRED )
add_executable( ObjectOriented-inlineDisabled src/utils/TimeProfiler.cpp src/mainOOP.cpp src/benchOOP/BenchOOP.cpp src/convolutionBench/BenchManager.cpp src/convolutionBench/IConvBench.cpp src/imProc/Image.cpp src/imProc/Matrix.cpp  src/imProc/Point.cpp  src/imProc/StructuringElement.cpp src/benchOOP/ErosionBenchOOP.cpp src/benchOOP/DilationBenchOOP.cpp src/benchOOP/OpeningBenchOOP.cpp src/benchOOP/ClosureBenchOOP.cpp )
target_link_libraries( ObjectOriented-inlineDisabled ${OpenCV_LIBS} )
set_target_properties(ObjectOriented-inlineDisabled PROPERTIES COMPILE_DEFINITIONS "DISABLE_INLINE")





