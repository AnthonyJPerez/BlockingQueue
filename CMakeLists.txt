cmake_minimum_required(VERSION 3.7)
project(BlockingQueue)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp inc/BlockingQueue.hpp inc/Logger.h src/Logger.cpp)
add_executable(BlockingQueue ${SOURCE_FILES})
