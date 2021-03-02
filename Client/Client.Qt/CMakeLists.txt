set(TARGET Client.Qt)
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


 
set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui)
 
add_executable(${TARGET} ${SOURCE_FILES})
 
target_link_libraries(${TARGET} 
    PRIVATE Qt5::Widgets)

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT})
                                
target_link_options(${TARGET} PRIVATE "-lxcb")
