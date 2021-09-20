find_library(LIB_SENSORS sensors)

add_library(fannn-shared governor.cpp lm_sensors_reader.cpp sysfs_pwm_writer.cpp user_config.cpp atomic_file_writer.cpp tokenizer.cpp profile_engine.cpp)

target_link_libraries(fannn-shared PRIVATE "${LIB_SENSORS}")
target_include_directories(fannn-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)