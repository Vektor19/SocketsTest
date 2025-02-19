cmake_minimum_required(VERSION 3.10)
project(Network)

add_library(Network STATIC src/network.cpp src/network.h)
target_include_directories(Network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
