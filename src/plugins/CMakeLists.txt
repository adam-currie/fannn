add_library(fannn-plugins STATIC
    dl_plugin_helper.cpp
    dl_obj.cpp
)

target_include_directories(fannn-plugins PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers)

set_property(TARGET fannn-plugins PROPERTY POSITION_INDEPENDENT_CODE ON)

target_link_libraries(fannn-plugins PRIVATE
    fannn-util dl
)


target_link_libraries(fannn-plugins PUBLIC
    fannn-file-io
)
