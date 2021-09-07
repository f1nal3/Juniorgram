project(DataAccess LANGUAGES CXX)

add_subdirectory(Public)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Postgre)
add_subdirectory(DataAccess.Repository)
add_subdirectory(DataAccess.SQLite)
add_subdirectory(DataAccess.Test)

set(TARGET DataAccess)

add_library(${TARGET} INTERFACE)
