cmake_minimum_required(VERSION 3.16)
project(cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(cpp
        ocio.h
        ocio.cpp
        shared_ptr_map.h
        colorspace.cpp
        config.cpp
        context.cpp
        processor.cpp
        transform.cpp storage.h ocio_abi.h)
