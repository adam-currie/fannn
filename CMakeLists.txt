cmake_minimum_required(VERSION 3.14...3.18)#qt6 requires 3.14
project(fannn   VERSION 0.1.0 
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(CTest)
include(FetchContent)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/util)
add_subdirectory(src/file-io)
add_subdirectory(src/plugins)
add_subdirectory(src/sensors)
add_subdirectory(src/lm-sensors-reader)
add_subdirectory(src/shared)
add_subdirectory(src/daemon)
add_subdirectory(src/gui)
add_subdirectory(src/tests)
