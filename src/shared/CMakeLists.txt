find_library(LIB_SENSORS sensors)

add_library(fannn-shared SHARED
    governor.cpp
    lm_sensors_reader.cpp
    sysfs_pwm_writer.cpp
    atomic_file_writer.cpp
    tokenizer.cpp
    profile_engine.cpp
    profile_persister.cpp
    composite_sensor_reader.cpp
    profile.cpp
    curve.cpp
)

set_property(TARGET fannn-shared PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(fannn-shared PRIVATE "${LIB_SENSORS}")
target_include_directories(fannn-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)
