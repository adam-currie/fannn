set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#todo set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Qml REQUIRED)
find_package(Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

qt6_add_executable(fannn-gui)

qt6_add_qml_module(fannn-gui
    URI Fannn
    VERSION 1.0
    SOURCES
        main.cpp
        sensor_list_model.h
        governor_list_model.h
        curve_list_model.h
        controller_list_model.h
        profile_list_model.h
        profile_model.h
        q_governor_error.h
        error_highlighter.h
        curve_model.h
        horz_extended_xy_model_wrapper.h
        sensor_list_model.cpp
        governor_list_model.cpp
        curve_list_model.cpp
        controller_list_model.cpp
        profile_list_model.cpp
        profile_model.cpp
        error_highlighter.cpp
        curve_model.cpp
        horz_extended_xy_model_wrapper.cpp
    QML_FILES
        main.qml
        Warning.qml
        Collapsible.qml
        SensorDelegate.qml
        CurveDelegate.qml
        GovernorDelegate.qml
        ControllerDelegate.qml
        ProfilesComboBox.qml
        UnsavedChangesDialog.qml
        SaveAnywayDialog.qml
        SpacedGridView.qml
        SpacedGridDelegate.qml
        STextField.qml
        GridViewCellPredictor.qml
        IdentifierValidator.qml
        AddButton.qml
        CurveChartView.qml
        SSoftValidatedField.qml
        NoneOptionComboBox.qml
)

target_link_libraries(fannn-gui PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Qml
    Qt6::Gui
    Qt6::Widgets
    fannn-shared
)

set_property(TARGET fannn-gui PROPERTY POSITION_INDEPENDENT_CODE ON)
