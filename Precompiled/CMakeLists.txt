project(Precompiled LANGUAGES CXX)

add_subdirectory(Catch2)

set(TARGET Precompiled)

add_library(${TARGET} INTERFACE)
