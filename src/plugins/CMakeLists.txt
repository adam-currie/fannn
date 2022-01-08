add_library(fannn-plugins INTERFACE)

target_include_directories(fannn-plugins INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

target_link_libraries(fannn-plugins INTERFACE
    fannn-util dl
)
