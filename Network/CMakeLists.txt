project(Network LANGUAGES CXX)

add_subdirectory(Public)
add_subdirectory(Network.Test)
add_subdirectory(Network.Static)

set(TARGET Network)

add_library(${TARGET} INTERFACE)
