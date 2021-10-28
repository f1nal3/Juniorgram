project(Server LANGUAGES CXX)

set(TARGET Server)
set(FOLDER Server)

add_subdirectory(Server.Exec)
add_subdirectory(Server.Test)
add_subdirectory(Server.Core)

add_library(${TARGET} INTERFACE)
