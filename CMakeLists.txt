cmake_minimum_required(VERSION 3.16)

project(Graphics VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    header/mainwindow.h
    src/canvas.cpp
    header/canvas.h
    src/titleBar.cpp
    header/titleBar.h
    mainwindow.ui
    resource.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Graphics
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resource.qrc
    )
    target_include_directories(Graphics PUBLIC header)
#    set_property(TARGET Graphics APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
else()
    if(ANDROID)
        add_library(Graphics SHARED
            ${PROJECT_SOURCES}
        )
        target_include_directories(Graphics PUBLIC header)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Graphics
            ${PROJECT_SOURCES}
        )
        target_include_directories(Graphics PUBLIC header)
    endif()
endif()

target_link_libraries(Graphics PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Graphics)
endif()
set_target_properties(Graphics PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Graphics
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Graphics)
endif()
