project(Server LANGUAGES CXX)

set(TARGET Server)
set(FOLDER Server)

add_subdirectory(Server.Exec)
#return back in stanislav pr
#add_subdirectory(Server.Test)
add_subdirectory(Server.Core)

add_library(${TARGET} INTERFACE)
