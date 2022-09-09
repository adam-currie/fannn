add_library(fannn-sysfs-pwm-writer SHARED
    sysfs_pwm_writer.cpp
)

target_compile_options(fannn-sysfs-pwm-writer PRIVATE -fno-gnu-unique)

set_property(TARGET fannn-sysfs-pwm-writer PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(fannn-sysfs-pwm-writer PRIVATE
    fannn-device-writers
)

set_target_properties(fannn-sysfs-pwm-writer
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins
    PREFIX ""
    OUTPUT_NAME "sysfs-pwm-writer"
    SUFFIX ".plugin"
)

target_include_directories(fannn-sysfs-pwm-writer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
