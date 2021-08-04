set(TARGET Precompiled.Catch2)
set(OUTPUT_NAME Precompiled.Catch2)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_library(Precompiled.Catch2.Main STATIC CatchMainConfig.cpp)
target_include_directories(Precompiled.Catch2.Main PUBLIC "${CONAN_CATCH2_ROOT}/include/catch2")

add_executable(Precompiled.Catch2.Test CatchDummyTest.cpp)
target_link_libraries(Precompiled.Catch2.Test Precompiled.Catch2.Main)

target_compile_definitions(Precompiled.Catch2.Test PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(Precompiled.Catch2.Test PRIVATE "${CONAN_CATCH2_ROOT}/include/catch2/catch.hpp")
