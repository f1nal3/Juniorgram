project(Server.Core LANGUAGES CXX)

add_subdirectory(Server.Core.Static)

set(TARGET Server.Core)

add_library(${TARGET} INTERFACE)
