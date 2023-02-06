set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt6_add_executable(fannn-permissions-dlg)

qt6_add_qml_module(fannn-permissions-dlg
    URI FannnPermissionsDlg
    VERSION 1.0
    SOURCES
        main.cpp
    QML_FILES
        main.qml
)

target_link_libraries(fannn-permissions-dlg PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Gui
    Qt6::Widgets
)

set_property(TARGET fannn-permissions-dlg PROPERTY POSITION_INDEPENDENT_CODE ON)
