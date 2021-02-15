cmake_minimum_required( VERSION 3.10.0 )

project(Seialize_lib VERSION 1.0)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(SOURCE
    "./src/Serialize.cpp")

set(HEADERS
    "./include/Serialize.h")

# Add a library with the above sources
add_library(${PROJECT_NAME} STATIC ${SOURCE} ${HEADERS})
add_library(sub::lib_serialize ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
)

target_link_libraries(Seialize_lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_include_directories(
        ${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/../Gui_Qt")
