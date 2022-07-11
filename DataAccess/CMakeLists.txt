set(TARGET DataAccess)
set(FOLDER ${TARGET})

project(${TARGET} LANGUAGES CXX)

add_subdirectory(Public)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Postgre)
add_subdirectory(DataAccess.SQLite)
add_subdirectory(DataAccess.Test)


add_library(${TARGET} INTERFACE)
