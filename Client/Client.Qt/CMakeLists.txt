set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES
        Widgets/BioButton.cpp
        Widgets/BioButton.hpp
        Widgets/CaptionButton.hpp
        Widgets/CaptionButton.cpp
        Widgets/ChannelListWidget.cpp
        Widgets/ChannelListWidget.hpp
        Widgets/ChannelListWindow.cpp
        Widgets/ChannelListWindow.hpp
        Widgets/ComboBox.hpp
        Widgets/ComboBox.cpp
        Widgets/FlatButton.cpp
        Widgets/FlatButton.hpp
        Widgets/InputFields.hpp
        Widgets/InputFields.cpp
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
        Style/Shadow.hpp
        Style/Style.cpp
        Style/Style.hpp
        App.cpp
        App.hpp
        ChatWindow.cpp
        ChatWindow.hpp
        login.hpp
        login.cpp
        main.cpp
	ConnectionManager.hpp
        MainWidget.cpp
        MainWidget.hpp
        pch.hpp
        registration.hpp
        registration.cpp)


add_executable(${TARGET} ${SOURCE_FILES}
        resources/resources.qrc)

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32 winmm dwmapi)
elseif (UNIX AND NOT APPLE)
    list(APPEND EXTRA_LIBS xcb)
endif ()

target_link_libraries(${TARGET}
        PRIVATE Qt5::Widgets ${EXTRA_LIBS} CONAN_PKG::asio
        PUBLIC Network.Public Client.Core.Static)

target_precompile_headers(${TARGET} PRIVATE pch.hpp)

if (WIN32)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        file(COPY "${CONAN_BIN_DIRS_QT}/Qt5Cored.dll" "${CONAN_BIN_DIRS_QT}/Qt5Guid.dll"
                "${CONAN_BIN_DIRS_QT}/Qt5Widgetsd.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qminimald.dll"
                "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qoffscreend.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qwindowsd.dll"
                DESTINATION "${CMAKE_BINARY_DIR}/bin")
        message("Install Debug .dll's")
    elseif (CMAKE_BUILD_TYPE MATCHES Release)
        file(COPY "${CONAN_BIN_DIRS_QT}/Qt5Core.dll" "${CONAN_BIN_DIRS_QT}/Qt5Gui.dll"
                "${CONAN_BIN_DIRS_QT}/Qt5Widgets.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qminimal.dll"
                "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qoffscreen.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qwindows.dll"
                DESTINATION "${CMAKE_BINARY_DIR}/bin")
        message("Install Release .dll's")
    endif ()
    file(DOWNLOAD
            "https://drive.google.com/u/0/uc?id=1_SrQNoI8lwsMD6bXFnQBAzP7PWGVeilo&export=download"
            "${CMAKE_BINARY_DIR}/bin/glib-2.0-0.dll"
            SHOW_PROGRESS)
endif ()
