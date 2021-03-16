project(Client LANGUAGES CXX)

add_subdirectory(Client.Core)
add_subdirectory(Client.Qt)
add_subdirectory(Client.CLI)

set(TARGET Client)
add_library(${TARGET} INTERFACE)