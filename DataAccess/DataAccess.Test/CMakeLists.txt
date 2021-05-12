set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

set(SOURCE_FILES
    DataAccessTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC
    CONAN_PKG::catch2
    CONAN_PKG::libpq
    CONAN_PKG::libpqxx
    DataAccess.Static)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
