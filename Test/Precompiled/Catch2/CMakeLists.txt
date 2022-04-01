set(TARGET_MAIN Precompiled.Catch2.Main)
set(TARGET_TEST Precompiled.Catch2.Test)

set(OUTPUT_NAME Precompiled.Catch2)

add_library(${TARGET_MAIN} STATIC CatchMainConfig.cpp)
target_link_libraries(${TARGET_MAIN} PUBLIC Catch2::Catch2)

add_executable(${TARGET_TEST} CatchDummyTest.cpp)

set_target_properties(${TARGET_MAIN} PROPERTIES FOLDER ${FOLDER})
set_target_properties(${TARGET_TEST} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET_TEST} ${TARGET_MAIN})

target_compile_definitions(${TARGET_TEST} PRIVATE CATCH_CONFIG_FAST_COMPILE)

# TODO This is definately bad, gotta rework this ASAP
get_target_property(CATCH2_INCLUDE_DIRS Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
string(CONCAT CATCH2_INCLUDE ${CATCH2_INCLUDE_DIRS} "/catch2/catch.hpp")

target_precompile_headers(${TARGET_TEST} PRIVATE ${CATCH2_INCLUDE})
