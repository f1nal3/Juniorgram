set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES
        main.cpp
        Widgets/caption_button.h
        Widgets/caption_button.cpp
        mainwidget.cpp
        mainwidget.h
        Style/style.h
        Style/style.cpp
        Style/shadow.cpp
        Style/shadow.h)

add_executable(${TARGET} ${SOURCE_FILES}
        resources/resources.qrc)

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32 winmm dwmapi)
elseif (UNIX)
    list(APPEND EXTRA_LIBS xcb)
endif ()

target_link_libraries(${TARGET}
        PRIVATE Qt5::Widgets ${EXTRA_LIBS})

target_include_directories(${TARGET}
        PRIVATE ${CONAN_INCLUDE_DIRS_QT})
