set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(STYLE_SOURCES
        Style/Shadow.hpp
        Style/Style.cpp
        Style/Style.hpp
        Style/StyleColor.cpp
        Style/StyleColor.hpp
        Style/StyleFont.cpp
        Style/StyleFont.hpp
        Style/StyleIcon.cpp
        Style/StyleIcon.hpp
        Style/StyleScale.cpp
        Style/StyleScale.hpp
        Style/StyleTypes.cpp
        Style/StyleTypes.hpp

        Style/Palette.cpp
        Style/Palette.hpp
        Style/StyleBasic.hpp
        Style/StyleBasic.cpp
        Style/StyleIcons.cpp
        Style/StyleIcons.hpp
        Style/Styles.hpp
        Style/StyleWidgets.hpp
        Style/StyleWidgets.cpp)

set(UI_WIDGETS_SOURCES
        Widgets/AbstractButton.cpp
        Widgets/AbstractButton.hpp
        Widgets/Buttons.cpp
        Widgets/Buttons.hpp
        Widgets/ComboBox.cpp
        Widgets/ComboBox.hpp
        Widgets/InputFields.cpp
        Widgets/InputFields.hpp
        Widgets/Label.cpp
        Widgets/Label.hpp
        Widgets/ListWidget.cpp
        Widgets/ListWidget.hpp
        Widgets/LoadingSpinner.cpp
        Widgets/LoadingSpinner.hpp
        Widgets/LogoWidget.cpp
        Widgets/LogoWidget.hpp
        Widgets/Menu.cpp
        Widgets/Menu.hpp
        Widgets/MenuItem.cpp
        Widgets/MenuItem.hpp
        Widgets/MessageWidget.cpp
        Widgets/MessageWidget.hpp
        Widgets/ReplyWidget.cpp
        Widgets/ReplyWidget.hpp
        Widgets/ReplyMessageWidget.cpp
        Widgets/ReplyMessageWidget.hpp
        Widgets/Page.cpp
        Widgets/Page.hpp
        Widgets/PopupWidget.cpp
        Widgets/PopupWidget.hpp
        Widgets/ScrollArea.cpp
        Widgets/ScrollArea.hpp
        Widgets/TextEdit.cpp
        Widgets/TextEdit.hpp
        Widgets/TitleWidget.cpp
        Widgets/TitleWidget.hpp)

set(PAGES_SOURCES
        Pages/ChannelCreationPage.cpp
        Pages/ChannelCreationPage.hpp
        Pages/ChannelListPage.cpp
        Pages/ChannelListPage.hpp
        Pages/ChatPage.cpp
        Pages/ChatPage.hpp
        Pages/LoginPage.cpp
        Pages/LoginPage.hpp
        Pages/RegistrationPage.cpp
        Pages/RegistrationPage.hpp)

set(SOURCE_FILES
        Widgets/ChannelListWidget.cpp
        Widgets/ChannelListWidget.hpp
        Widgets/ChatHistory.cpp
        Widgets/ChatHistory.hpp
        Widgets/ChatWidget.cpp
        Widgets/ChatWidget.hpp
        Widgets/ChannelBar.cpp
        Widgets/ChannelBar.hpp
        Widgets/MessageWidget.cpp
        Widgets/MessageWidget.hpp
        Widgets/ReactionLayout.cpp
        Widgets/ReactionLayout.hpp

        Application.cpp
        Application.hpp
        ConnectionManager.cpp
        ConnectionManager.hpp

        main.cpp
        MainWidget.cpp
        MainWidget.hpp
        Settings.cpp
        Settings.hpp
        SettingsWidget.cpp
        SettingsWidget.hpp)

add_executable(${TARGET} ${SOURCE_FILES} ${STYLE_SOURCES} ${UI_WIDGETS_SOURCES} ${PAGES_SOURCES}
        resources/resources.qrc)

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32 winmm dwmapi)
elseif (UNIX AND NOT APPLE)
    list(APPEND EXTRA_LIBS xcb)
endif ()

target_link_libraries(${TARGET}
        PRIVATE Qt::Widgets Qt::Gui ${EXTRA_LIBS} CONAN_PKG::asio
        PUBLIC Network.Public Client.Core.Static Utility.Public DataAccess.SQLite)


target_include_directories(${TARGET}
        PUBLIC $<TARGET_PROPERTY:DataAccess.SQLite,SOURCE_DIR>/..)

# This needs to be an install or after install operation
# At this point it disrupts CI build so I commented it out =)

#if (WIN32)
#   set(Qt5_DIR $ENV{CMAKE_PREFIX_PATH})
#    message(STATUS ${Qt5_DIR})
#    if (CMAKE_BUILD_TYPE MATCHES Debug)
#        file(COPY "${Qt5_DIR}/bin/Qt5Cored.dll" "${Qt5_DIR}/bin/Qt5Guid.dll"
#                "${Qt5_DIR}/bin/Qt5Widgetsd.dll" "${Qt5_DIR}/bin/Qt5Sqld.dll"
#                "${Qt5_DIR}/plugins/platforms/qminimald.dll" "${Qt5_DIR}/plugins/platforms/qoffscreend.dll"
#                "${Qt5_DIR}/plugins/platforms/qwindowsd.dll"
#                DESTINATION "${CMAKE_BINARY_DIR}/bin")
#        message("Install Debug .dll's")
#    elseif (CMAKE_BUILD_TYPE MATCHES Release)
#        file(COPY "${Qt5_DIR}/bin/Qt5Core.dll" "${Qt5_DIR}/bin/Qt5Gui.dll"
#                "${Qt5_DIR}/bin/Qt5Widgets.dll" "${Qt5_DIR}/bin/Qt5Sqld.dll"
#                "${Qt5_DIR}/plugins/platforms/qminimal.dll" "${Qt5_DIR}/plugins/platforms/qoffscreen.dll"
#                "${Qt5_DIR}/plugins/platforms/qwindows.dll"
#                DESTINATION "${CMAKE_BINARY_DIR}/bin")
#        message("Install Release .dll's")
#    endif ()
#    file(DOWNLOAD
#            "https://drive.google.com/u/0/uc?id=1_SrQNoI8lwsMD6bXFnQBAzP7PWGVeilo&export=download"
#            "${CMAKE_BINARY_DIR}/bin/glib-2.0-0.dll"
#            SHOW_PROGRESS)
#endif ()
