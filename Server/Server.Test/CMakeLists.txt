set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
    ArgumentParserTests.cpp
    ServerTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    CONAN_PKG::catch2
    Network.Public 
    Server.Core.Static)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
