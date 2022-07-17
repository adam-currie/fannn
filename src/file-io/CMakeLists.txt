add_library(fannn-file-io STATIC
    atomic_file_writer.cpp
    proximate_temp_file.cpp
)

set_property(TARGET fannn-file-io PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fannn-file-io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

