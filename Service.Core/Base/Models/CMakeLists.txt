project(Models LANGUAGES CXX)

add_subdirectory(Public)

set(TARGET Models)

add_library(${TARGET} INTERFACE)
