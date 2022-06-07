set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES 
    ExceptionTest.cpp
    UtilityTest.cpp
    )

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PUBLIC 
    Precompiled.Catch2.Main
    Network.Public
    Utility.Public)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
