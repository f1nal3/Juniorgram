cmake_minimum_required(VERSION 3.10.0)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

project(Gui_Qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(TS_FILES Gui_Qt_en_150.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

set(SOURCES ${SOURCES})

## if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
##    qt_add_executable(Gui_Qt
##      ${PROJECT_SOURCES})
##    else()
        add_executable(Gui_Qt  ${PROJECT_SOURCES})
##endif()

# qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_include_directories(Gui_Qt PRIVATE ${CONAN_INCLUDE_DIRS_QT})

target_link_libraries(Gui_Qt PUBLIC Qt5::Widgets)



# target_link_libraries(${PROJECT_NAME} PRIVATE
#     sub::lib_serialize
# )