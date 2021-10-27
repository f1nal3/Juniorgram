set(TARGET_MAIN Precompiled.Catch2.Main)
set(TARGET_TEST Precompiled.Catch2.Test)

set(OUTPUT_NAME Precompiled.Catch2)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(${TARGET_MAIN} STATIC CatchMainConfig.cpp)
target_include_directories(${TARGET_MAIN} PUBLIC "${CONAN_CATCH2_ROOT}/include/catch2")

add_executable(${TARGET_TEST} CatchDummyTest.cpp)

set_target_properties(${TARGET_MAIN} PROPERTIES FOLDER ${FOLDER})
set_target_properties(${TARGET_TEST} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET_TEST} ${TARGET_MAIN})

target_compile_definitions(${TARGET_TEST} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET_TEST} PRIVATE "${CONAN_CATCH2_ROOT}/include/catch2/catch.hpp")