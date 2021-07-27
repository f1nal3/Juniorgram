set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
    ArgumentParserTests.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    Precompiled.Catch2.Main
    Network.Public 
    Server.Core.Static)

target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
