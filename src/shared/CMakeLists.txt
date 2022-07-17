add_library(fannn-shared SHARED
    governor.cpp
    sysfs_pwm_writer.cpp
    tokenizer.cpp
    profile_engine.cpp
    profile_persister.cpp
    composite_device_writer.cpp
    profile.cpp
    curve.cpp
    profile_governor_exec_context.cpp
    expression.cpp
)

set_property(TARGET fannn-shared PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(fannn-shared PUBLIC
    fannn-sensors
)

target_link_libraries(fannn-shared PRIVATE
    fannn-util
    fannn-file-io
)

target_include_directories(fannn-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
