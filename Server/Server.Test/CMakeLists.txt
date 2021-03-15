set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
    ServerTest.cpp
    ArgumentParserTests.cpp)

set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC Network.Public CONAN_PKG::catch2)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
