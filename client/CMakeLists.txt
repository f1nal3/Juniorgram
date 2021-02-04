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
	../common/client.h
	../common/WarningSuppressing.h)

add_executable(client ${SOURCES} ${HEADERS})

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

source_group("Client"         FILES ${SOURCES})
source_group("Network"        FILES ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} CONAN_PKG::asio)

target_include_directories(
	client PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/../common")

install(TARGETS client
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS client
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)