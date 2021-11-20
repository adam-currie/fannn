add_executable(fannn-daemon
        daemon.cpp
        min_interval_stepper.cpp)
target_link_libraries(fannn-daemon PRIVATE fannn-shared)
target_include_directories(fannn-daemon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
