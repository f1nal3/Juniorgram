set(TARGET DataAccess.Static)
set(OUTPUT_NAME DataAccess.Static)

set(SOURCE_FILES
    mainDBAPI.cpp
    DataBasePostgreSQL.hpp)

find_package(PostgreSQL REQUIRED)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE
    CONAN_PKG::libpqxx
    CONAN_PKG::libpq)
target_link_libraries(${TARGET} PUBLIC
    Utility.Public
    DataAccess.Public)
