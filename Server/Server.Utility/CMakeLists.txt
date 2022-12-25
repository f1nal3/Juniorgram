project(Server.Utility LANGUAGES CXX)

set(TARGET Server.Utility)

add_subdirectory(ServerUtility)

add_library(${TARGET} INTERFACE)
