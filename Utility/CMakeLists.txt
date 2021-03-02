project(Utility LANGUAGES CXX)

add_subdirectory(Utility.Public)
add_subdirectory(Utility.Static)

set(TARGET Utility)

add_library(${TARGET} INTERFACE)
