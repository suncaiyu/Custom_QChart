cmake_minimum_required(VERSION 3.5)

project(TimeLineWidget VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 QUIET  COMPONENTS Widgets)
if (${QT_FOUND})
else()
    if (UNIX)
        set(CMAKE_PREFIX_PATH "/data/system/liucj/Qt/5.15.2/gcc_64/lib/cmake/Qt5/")
    elseif(WIN32)
        set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake\\Qt5")
    endif()
    find_package(QT NAMES Qt6 Qt5 REQUIRED  COMPONENTS Widgets)
endif()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        TimelineWidget.h TimelineWidget.cpp
        TimelineContext.h TimelineContext.cpp
        ChartWidget.h ChartWidget.cpp
        ChartData.h ChartData.cpp
        res.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(TimeLineWidget
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TimeLineWidget APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TimeLineWidget SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TimeLineWidget
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(TimeLineWidget PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(TimeLineWidget PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS TimeLineWidget
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TimeLineWidget)
endif()
