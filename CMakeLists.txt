cmake_minimum_required(VERSION 2.6) 
project(CellCount)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(CellCount)


add_executable(CellCount
        CellCount/Main.cpp
        CellCount/ImageProcess.h
        CellCount/ImageProcess.cpp)


target_link_libraries(CellCount ${OpenCV_LIBS} )
