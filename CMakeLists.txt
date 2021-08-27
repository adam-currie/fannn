cmake_minimum_required(VERSION 3.7...3.18)
project(fannn   VERSION 0.1.0 
                LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(CTest)
include(FetchContent)

add_subdirectory(src/daemon)
#todo add_subdirectory(src/gui)
add_subdirectory(src/shared)
add_subdirectory(src/tests)