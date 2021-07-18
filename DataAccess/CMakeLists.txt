project(DataAccess LANGUAGES CXX)

add_subdirectory(DataAccess.Static)
add_subdirectory(DataAccess.Debug)
add_subdirectory(DataAccess.Test)

set(TARGET DataAccess)

add_library(${TARGET} INTERFACE)
