set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core Quick QuickControls2 REQUIRED)

if(${QT_VERSION_MAJOR} LESS ${MIN_QT_VERSION_MAJOR})
    message(FATAL_ERROR "qt version < ${MIN_QT_VERSION_MAJOR}")
endif()

qt_add_executable(fannn-gui MANUAL_FINALIZATION main.cpp qml.qrc)

target_compile_definitions(fannn-gui
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(fannn-gui
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::QuickControls2)

qt_import_qml_plugins(fannn-gui)
qt_finalize_executable(fannn-gui)
