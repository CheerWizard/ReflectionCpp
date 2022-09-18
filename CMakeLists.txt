cmake_minimum_required(VERSION 3.20)
project(ReflectionCpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(ReflectionCpp main.cpp types.cpp reflection.h types.h template_helper.h CodeGenerator.h CodeGenerator.cpp)
