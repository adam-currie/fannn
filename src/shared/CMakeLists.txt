add_library(fannn-shared governor.cpp reader.cpp)
target_include_directories(fannn-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)