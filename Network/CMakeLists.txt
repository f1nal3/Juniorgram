project(Network LANGUAGES CXX)

add_subdirectory(Public)
add_subdirectory(Network.Static)
add_subdirectory(Network.Test)

set(TARGET Network)

add_library(${TARGET} INTERFACE)
