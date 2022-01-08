add_library(fannn-sensors SHARED
    plugins_composite_sensor_reader.cpp
)

set_property(TARGET fannn-sensors PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fannn-sensors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

target_link_libraries(fannn-sensors PUBLIC
    fannn-plugins
)
