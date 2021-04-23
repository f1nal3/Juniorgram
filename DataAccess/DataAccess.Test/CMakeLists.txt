set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

add_definitions(-D_ITERATOR_DEBUG_LEVEL=2)

set(SOURCE_FILES
    FileAdapterTest.cpp)
#    DataAccessTest.cpp)

set(CMAKE_BUILD_TYPE DEBUG) 

add_executable(${TARGET} ${SOURCE_FILES})


target_link_libraries(${TARGET} PUBLIC
    DataAccess.Static
    DataAccess.Debug
    CONAN_PKG::catch2
    CONAN_PKG::libpq
    CONAN_PKG::libpqxx)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
