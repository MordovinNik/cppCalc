cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project("newMath")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY &{CMAKE_CURRENT_SOURCE_DIR}/../build/bin/plugins)

add_library(newMath SHARED newMath.cpp)