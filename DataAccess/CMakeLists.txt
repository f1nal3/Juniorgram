project(DataAccess LANGUAGES CXX)

set(TARGET DataAccess)
set(FOLDER "DataAccess")

add_subdirectory(Public)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Postgre)
# This relies heavily on Qt and therefore is put on hold.
#add_subdirectory(DataAccess.SQLite)
add_subdirectory(DataAccess.Test)


add_library(${TARGET} INTERFACE)
