cmake_minimum_required(VERSION 3.15)
project(IG_T3)

#set(CMAKE_CXX_FLAGS_DEBUG "-O3")
#set(CMAKE_CXX_FLAGS_RELEASE  "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(IG_T3
        main.cpp)
