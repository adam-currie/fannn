find_library(LIB_SENSORS sensors)

add_library(fannn-lm-sensors-reader SHARED
    lm_sensors_reader.cpp
    id_builder.cpp
)

set_property(TARGET fannn-lm-sensors-reader PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(fannn-lm-sensors-reader PRIVATE
    "${LIB_SENSORS}"
    fannn-sensors
    fannn-util
)

set_target_properties(fannn-lm-sensors-reader
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
    PREFIX ""
    OUTPUT_NAME "lmsensors-reader"
    SUFFIX ".plugin"
)

target_include_directories(fannn-lm-sensors-reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
