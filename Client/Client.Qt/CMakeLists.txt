set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(SOURCE_FILES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    login.cpp
    login.h
    registration.cpp
    registration.h
    )
 
add_executable(${TARGET} ${SOURCE_FILES})
 
target_link_libraries(${TARGET} 
    PRIVATE Qt5::Widgets)

if(NOT APPLE)
    target_link_options(${TARGET} PRIVATE "-lxcb")
endif()
