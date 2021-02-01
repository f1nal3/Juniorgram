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
	../common/server.h)

add_executable(server ${SOURCES} ${HEADERS})

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

source_group("Server"         FILES ${SOURCES})
source_group("Network"        FILES ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} CONAN_PKG::asio)

target_include_directories(
	server PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../common)

install(TARGETS server
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Debug)
install(TARGETS server
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Release)