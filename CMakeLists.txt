cmake_minimum_required(VERSION 3.30)
project(28 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

#include_directories(RandomNameLib/inc)
#add_library(genNames STATIC RandomNameLib/inc/randomName.hpp RandomNameLib/src/randomName.cpp)
add_subdirectory(1)
add_subdirectory(2)
add_subdirectory(3)