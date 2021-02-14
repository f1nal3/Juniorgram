cmake_minimum_required(VERSION 3.15.0)

project(DataBaseAPI)

set(SOURCES
	mainDBAPI.cpp)
	
#set(HEADERS
#	*.h)
	
add_executable(DataBaseAPI ${SOURCES})

find_package(PostgreSQL REQUIRED)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} CONAN_PKG::libpqxx)

#install(TARGETS DataBaseAPI
#		CONFIGURATIONS Debug
#		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/DB/Debug)
#install(TARGETS DataBaseAPI
#		CONFIGURATIONS Release
#		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/DB/Release)