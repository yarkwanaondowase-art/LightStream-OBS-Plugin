cmake_minimum_required(VERSION 3.28)

project(lightstream VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lightstream MODULE
    src/lightstream.cpp
)

set_target_properties(lightstream PROPERTIES
    PREFIX ""
    OUTPUT_NAME "lightstream"
)
