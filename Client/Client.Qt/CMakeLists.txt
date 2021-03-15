set(TARGET Client.Qt)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES
        main.cpp
        Widgets/caption_button.h
        Widgets/caption_button.cpp
        Widgets/input_fields.h
        Widgets/input_fields.cpp
        mainwidget.cpp
        mainwidget.h
        Style/style.h
        Style/style.cpp
        Style/shadow.cpp
        Style/shadow.h)

add_executable(${TARGET} ${SOURCE_FILES}
        resources/resources.qrc)

if(WIN32)
if(CMAKE_BUILD_TYPE MATCHES Debug)
	file(COPY "${CONAN_BIN_DIRS_QT}/Qt5Cored.dll" "${CONAN_BIN_DIRS_QT}/Qt5Guid.dll"
		"${CONAN_BIN_DIRS_QT}/Qt5Widgetsd.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qminimald.dll"
		"${CONAN_BUILD_DIRS_QT}/plugins/platforms/qoffscreend.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qwindowsd.dll"
		DESTINATION "${CMAKE_BINARY_DIR}/bin")
	message("Install Debug .dll's")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
	file(COPY "${CONAN_BIN_DIRS_QT}/Qt5Core.dll" "${CONAN_BIN_DIRS_QT}/Qt5Gui.dll"
		"${CONAN_BIN_DIRS_QT}/Qt5Widgets.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qminimal.dll"
		"${CONAN_BUILD_DIRS_QT}/plugins/platforms/qoffscreen.dll" "${CONAN_BUILD_DIRS_QT}/plugins/platforms/qwindows.dll"
		DESTINATION "${CMAKE_BINARY_DIR}/bin")
	message("Install Release .dll's")
endif()
file(DOWNLOAD
	"https://drive.google.com/u/0/uc?id=1_SrQNoI8lwsMD6bXFnQBAzP7PWGVeilo&export=download"
	"${CMAKE_BINARY_DIR}/bin/glib-2.0-0.dll"
	SHOW_PROGRESS)
endif()
