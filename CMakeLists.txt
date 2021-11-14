cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(PROJECT_NAME Calc)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Calculator.h
	src/Calculator.cpp
	src/FunctionManager.h
	src/FunctionManager.cpp
	src/Parser.h
	src/Parser.cpp
)

add_subdirectory(newMath)