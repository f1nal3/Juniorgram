set(TARGET Logger)

add_subdirectory(Public)
add_subdirectory(Logger.Static)

add_library(${TARGET} INTERFACE)
