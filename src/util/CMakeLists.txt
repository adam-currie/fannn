add_library(fannn-util STATIC
    atomic_file_writer.cpp
)

set_property(TARGET fannn-util PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fannn-util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
