set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

set(SOURCE_FILES
    DataAccessTest.cpp
    FileDBTest.cpp)

set(CMAKE_BUILD_TYPE DEBUG) 

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC
    DataAccess.Static
    DataAccess.Debug
    CONAN_PKG::libpq
    CONAN_PKG::libpqxx
    CONAN_PKG::catch2)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
