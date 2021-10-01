cmake_minimum_required(VERSION 3.14...3.18)#qt6 requires 3.14
project(fannn   VERSION 0.1.0 
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(FetchContent)

add_subdirectory(src/daemon)
add_subdirectory(src/gui)
add_subdirectory(src/shared)
add_subdirectory(src/tests)
