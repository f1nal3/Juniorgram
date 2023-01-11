project(DataAccess LANGUAGES CXX)

set(TARGET DataAccess)
set(FOLDER "DataAccess")

add_subdirectory(Public)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Postgre)
add_subdirectory(DataAccess.Test)


add_library(${TARGET} INTERFACE)
