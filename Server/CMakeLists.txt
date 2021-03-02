project(Server LANGUAGES CXX)

add_subdirectory(Server.Exec)

set(TARGET Server)

add_library(${TARGET} INTERFACE)
