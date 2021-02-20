cmake_minimum_required(VERSION 3.15.0)

project(DataBaseAPI)

set(SOURCES
	mainDBAPI.cpp)
	
set(HEADERS
	*.hpp)
	
add_executable(DataBaseAPI ${SOURCES})

find_package(PostgreSQL REQUIRED)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} CONAN_PKG::libpqxx CONAN_PKG::libpq)