set(TARGET DataAccess)
set(FOLDER Service.Core/DataAccess)

add_subdirectory(Public)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Postgre)
add_subdirectory(DataAccess.Test)
add_subdirectory(DataAccess.Mock)


add_library(${TARGET} INTERFACE)
