add_library(fannn-device-writers STATIC
    plugins_composite_device_writer.cpp
)

set_property(TARGET fannn-device-writers PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fannn-device-writers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

target_link_libraries(fannn-device-writers PRIVATE
    fannn-util
)

target_link_libraries(fannn-device-writers PUBLIC
    fannn-plugins
)
