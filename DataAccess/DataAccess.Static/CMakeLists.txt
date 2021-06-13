set(TARGET DataAccess.Static)
set(OUTPUT_NAME DataAccess.Static)

set(SOURCE_FILES
    DatabaseAbstractionLayout.hpp
    DatabaseAbstractionLayout.cpp
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepository.hpp
    PostgreRepository.cpp
    LiteAdapter.hpp
    LiteAdapter.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
    PRIVATE Qt5::Sql
    PUBLIC CONAN_PKG::libpq CONAN_PKG::libpqxx)


target_link_libraries(${TARGET} PUBLIC
    Utility.Public
    DataAccess.Public
    Network.Public)

