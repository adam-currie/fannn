add_library(fannn-shared SHARED
    governor.cpp
    tokenizer.cpp
    profile_engine.cpp
    profile_persister.cpp
    profile.cpp
    curve.cpp
    profile_governor_exec_context.cpp
    expression.cpp
)

set_property(TARGET fannn-shared PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(fannn-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

target_link_libraries(fannn-shared PUBLIC
    fannn-sensors
    fannn-device-writers
)

target_link_libraries(fannn-shared PRIVATE
    fannn-util
    fannn-file-io
)
