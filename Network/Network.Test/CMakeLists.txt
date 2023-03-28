set(TARGET Network.Test)
set(OUTPUT_NAME Network.Test)

set(SOURCE_FILES
        BinarySerializeTest.cpp
        TimestampTest.cpp)

if (WIN32)
    set(EXTRA_LIBS ws2_32 wsock32)
else ()
endif ()

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} asio::asio)

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public Precompiled.Catch2.Main)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})

install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${TEST_INSTALL_DIR})
