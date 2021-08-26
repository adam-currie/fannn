add_executable(fannn-daemon daemon.cpp)
target_link_libraries(fannn-daemon PRIVATE fannn-shared)