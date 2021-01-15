cmake_minimum_required(VERSION 3.10.0)

project(server)

set(SOURCES
	mainServer.cpp)

set(HEADERS
	../common/common.h
	../common/connection.h
	../common/message.h
	../common/safeQueue.h
	../common/network.h
	../common/server.h
	../common/asio.hpp)

add_executable(server ${SOURCES} ${HEADERS})

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

set_target_properties(server
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDART_REQUIRED ON
    CXX_EXTENSIONS off
    )

source_group("Server"         FILES ${SOURCES})
source_group("Network"        FILES ${HEADERS})

target_link_libraries(server PRIVATE ${EXTRA_LIBS})

target_include_directories(
	server PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../common
	${CMAKE_CURRENT_SOURCE_DIR}/../libs/asio-1.18.0/include)
