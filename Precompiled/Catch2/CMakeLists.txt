set(TARGET Precompiled.Catch2)
set(OUTPUT_NAME Precompiled.Catch2)

add_library(Precompiled.Catch2.Main STATIC CatchMainConfig.cpp)
target_include_directories(Precompiled.Catch2.Main PUBLIC "${CONAN_CATCH2_ROOT}/include/catch2")

add_executable(Precompiled.Catch2.Test CatchDummyTest.cpp)
target_link_libraries(Precompiled.Catch2.Test Precompiled.Catch2.Main)
target_precompile_headers(Precompiled.Catch2.Test PRIVATE "${CONAN_CATCH2_ROOT}/include/catch2/catch.hpp")
