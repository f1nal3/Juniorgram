project(Utility)

add_subdirectory(Utility.Public)
add_subdirectory(Utility.Static)
add_subdirectory(Utility.Test)

set(TARGET Utility)

add_library(${TARGET} INTERFACE)
