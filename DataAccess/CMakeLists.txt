project(DataAccess LANGUAGES CXX)

add_subdirectory(Public)
add_subdirectory(DataAccess.Static)
add_subdirectory(DataAccess.Test)

set(TARGET DataAccess)

add_library(${TARGET} INTERFACE)
