cmake_minimum_required(VERSION 3.10.0)

project(client)

set(SOURCES
	mainClient.cpp)

set(HEADERS
	../common/common.h
	../common/connection.h
	../common/message.h
	../common/safeQueue.h
	../common/network.h
	../common/client.h)

add_executable(client ${SOURCES} ${HEADERS})

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

set_target_properties(client
	PROPERTIES
    CXX_STANDARD 17
    CXX_STANDART_REQUIRED ON
    CXX_EXTENSIONS off)

source_group("Client"         FILES ${SOURCES})
source_group("Network"        FILES ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} CONAN_PKG::asio)


target_include_directories(
	client PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/../common")