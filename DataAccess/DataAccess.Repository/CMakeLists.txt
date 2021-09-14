project(DataAccess.Repository LANGUAGES CXX)

add_subdirectory(Client)
add_subdirectory(Public)
add_subdirectory(Server)

set(TARGET DataAccess.Repository)

add_library(${TARGET} INTERFACE)
