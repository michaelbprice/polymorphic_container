cmake_minimum_required(VERSION 2.8.4)
project(polymorphic_container)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(SOURCE_FILES main.cpp)
add_executable(polymorphic_container ${SOURCE_FILES})
set_property(TARGET polymorphic_container PROPERTY CXX_STANDARD 14)
