set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES
        Widgets/BioButton.cpp
        Widgets/BioButton.hpp
        Widgets/CaptionButton.cpp
        Widgets/CaptionButton.hpp
        Widgets/ChannelListWidget.cpp
        Widgets/ChannelListWidget.hpp
        Widgets/ChannelListWindow.cpp
        Widgets/ChannelListWindow.hpp
        Widgets/ComboBox.cpp
        Widgets/ComboBox.hpp
        Widgets/DateTimeEdit.cpp
        Widgets/DateTimeEdit.hpp
        Widgets/FlatButton.cpp
        Widgets/FlatButton.hpp
        Widgets/InputFields.cpp
        Widgets/InputFields.hpp
        Widgets/Label.cpp
        Widgets/Label.hpp
        Widgets/ListWidget.cpp
        Widgets/ListWidget.hpp
        Widgets/Menu.cpp
        Widgets/Menu.hpp
        Widgets/MenuItem.cpp
        Widgets/MenuItem.hpp
        Widgets/MessageWidget.cpp
        Widgets/MessageWidget.hpp
        Widgets/PopupWidget.cpp
        Widgets/PopupWidget.hpp
        Widgets/TextEdit.cpp
        Widgets/TextEdit.hpp
        Widgets/TimeEdit.cpp
        Widgets/TimeEdit.hpp
        Widgets/LogoWidget.cpp
        Widgets/LogoWidget.hpp
        Style/Style.cpp
        Style/Shadow.hpp
        Style/Style.hpp
        Application.cpp
        Application.hpp
        ChatWindow.cpp
        ChatWindow.hpp
        login.cpp
        login.hpp
        main.cpp
        MainWidget.cpp
        ConnectionManager.hpp
        MainWidget.hpp
        pch.hpp
        registration.cpp
        registration.hpp)

add_executable(${TARGET} ${SOURCE_FILES}
        resources/resources.qrc)

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32 winmm dwmapi)
elseif (UNIX AND NOT APPLE)
    list(APPEND EXTRA_LIBS xcb)
endif ()

target_link_libraries(${TARGET}
        PRIVATE Qt5::Widgets Qt5::Gui ${EXTRA_LIBS} CONAN_PKG::asio
        PUBLIC Network.Public Client.Core.Static Utility.Public)

target_precompile_headers(${TARGET} PRIVATE pch.hpp)

if (WIN32)
    set(Qt5_DIR $ENV{CMAKE_PREFIX_PATH})
    message(STATUS ${Qt5_DIR})
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        file(COPY "${Qt5_DIR}/bin/Qt5Cored.dll" "${Qt5_DIR}/bin/Qt5Guid.dll"
                "${Qt5_DIR}/bin/Qt5Widgetsd.dll" "${Qt5_DIR}/plugins/platforms/qminimald.dll"
                "${Qt5_DIR}/plugins/platforms/qoffscreend.dll" "${Qt5_DIR}/plugins/platforms/qwindowsd.dll"
                DESTINATION "${CMAKE_BINARY_DIR}/bin")
        message("Install Debug .dll's")
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        file(COPY "${Qt5_DIR}/bin/Qt5Core.dll" "${Qt5_DIR}/bin/Qt5Gui.dll"
                "${Qt5_DIR}/bin/Qt5Widgets.dll" "${Qt5_DIR}/plugins/platforms/qminimal.dll"
                "${Qt5_DIR}/plugins/platforms/qoffscreen.dll" "${Qt5_DIR}/plugins/platforms/qwindows.dll"
                DESTINATION "${CMAKE_BINARY_DIR}/bin")
        message("Install Release .dll's")
    endif ()
    file(DOWNLOAD
            "https://drive.google.com/u/0/uc?id=1_SrQNoI8lwsMD6bXFnQBAzP7PWGVeilo&export=download"
            "${CMAKE_BINARY_DIR}/bin/glib-2.0-0.dll"
            SHOW_PROGRESS)
endif ()
