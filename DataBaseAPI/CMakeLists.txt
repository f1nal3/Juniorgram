cmake_minimum_required(VERSION 3.15.0)

project(DataBaseAPI)

set(SOURCES
	./mainDBAPI.cpp
	./Sources/PostgreSQL.cpp
	./Sources/PostgreSQLTableOperations.cpp
	./Sources/PostgreSQLFieldOperations.cpp
	./Sources/PostgreSQLColumnOperations.cpp)
	
set(HEADERS
	./../common/WarningSuppressing.h
	./Headers/DataBasePostgreSQL.h
	./Headers/IDB.h)

add_executable(DataBaseAPI ${SOURCES} ${HEADERS})
	
find_package(PostgreSQL REQUIRED)

target_link_libraries(${PROJECT_NAME} CONAN_PKG::libpqxx CONAN_PKG::libpq)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
