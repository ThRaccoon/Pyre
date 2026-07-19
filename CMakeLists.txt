cmake_minimum_required(VERSION 3.20)
project(Pyre LANGUAGES CXX)

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "This project must be built on 64-bit")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Pyre)
add_subdirectory(PyreTests)
add_subdirectory(Sandbox)
