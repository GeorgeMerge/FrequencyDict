cmake_minimum_required(VERSION 3.0)

project(FrequencyDict)

set (CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCES
    src/main.cpp)

add_executable(freq ${PROJECT_SOURCES})
set_target_properties(freq PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")