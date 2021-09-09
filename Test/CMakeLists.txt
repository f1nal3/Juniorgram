project(TestLANGUAGES CXX)

add_subdirectory(PerformanceTest)
add_subdirectory(Precompiled)

set(TARGET Test)

add_library(${TARGET} INTERFACE)
