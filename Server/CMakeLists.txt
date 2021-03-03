project(Server LANGUAGES CXX)

add_subdirectory(Server.Exec)
add_subdirectory(Server.Test)

set(TARGET Server)

add_library(${TARGET} INTERFACE)
