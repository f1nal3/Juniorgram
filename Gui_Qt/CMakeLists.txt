cmake_minimum_required(VERSION 3.15)
 
project(Gui_Qt LANGUAGES CXX)
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
if (Qt5_FOUND)
        message( "Qt5 = ${Qt5_FOUND}" )
else()
        message(FATAL_ERROR "Qt not found!")
endif()


 
set(PROJECT_SOURCES
        ./main.cpp
        ./mainwindow.cpp
        ./mainwindow.h
        ./mainwindow.ui
)
 
add_executable(Gui_Qt ${PROJECT_SOURCES} )
 
target_link_libraries(Gui_Qt 
                     PRIVATE Qt5::Widgets
                     PRIVATE Utility::serialize)
target_include_directories(Gui_Qt 
                                PRIVATE ${CONAN_INCLUDE_DIRS_QT})