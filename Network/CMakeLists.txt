project(Network LANGUAGES CXX)

add_subdirectory(Public)

set(TARGET Network)

add_library(${TARGET} INTERFACE)
