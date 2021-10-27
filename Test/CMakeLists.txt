project(TestLANGUAGES CXX)

set(TARGET Test)
set(FOLDER Test)

add_subdirectory(PerformanceTest)
add_subdirectory(Precompiled)

add_library(${TARGET} INTERFACE)
