cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)


add_executable(main1 src/main1.cpp inc/main1.hpp)
target_include_directories(main1 PUBLIC inc)
