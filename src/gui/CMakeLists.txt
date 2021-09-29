set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#todo set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)

qt6_add_executable(fannn-gui)

qt6_add_qml_module(fannn-gui
    URI fannn
    VERSION 1.0
    SOURCES
        main.cpp
        sensors_model.h
        profiles_model.h
        sensors_model.cpp
        profiles_model.cpp
    QML_FILES
        main.qml
        Warning.qml
        Collapsible.qml
)

target_link_libraries(fannn-gui PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Gui
    fannn-shared
)

set_property(TARGET fannn-gui PROPERTY POSITION_INDEPENDENT_CODE ON)

#debug qt_finalize_executable(fannn-gui)

