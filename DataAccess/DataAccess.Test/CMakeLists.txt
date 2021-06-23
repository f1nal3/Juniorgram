set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

set(SOURCE_FILES
    DataAccessTest.cpp
    FileDBTest.cpp)
    
add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} PUBLIC
    CONAN_PKG::libpq
    CONAN_PKG::libpqxx
    CONAN_PKG::catch2
    DataAccess.Static
    DataAccess.Debug
    Utility.Public
    Utility.Static)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
