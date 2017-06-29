cmake_minimum_required(VERSION 2.8.3)

project(zmq-communication)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(cppzmq REQUIRED)
include_directories(${cppzmq_INCLUDE_DIR})

add_subdirectory(src)
