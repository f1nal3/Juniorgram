project(Client LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(Client.Core)
add_subdirectory(Client.Qt)
add_subdirectory(Client.CLI)

set(TARGET Client)
add_library(${TARGET} INTERFACE)
