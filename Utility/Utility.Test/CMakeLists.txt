set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES 
    ExceptionTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    Precompiled.Catch2.Main
    Network.Public
    Utility.Public
    Utility.Static)



target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
