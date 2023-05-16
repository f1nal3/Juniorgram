set(TARGET Logger)

add_subdirectory(Public)
add_subdirectory(Logger.Static)
add_subdirectory(Logger.Test)


add_library(${TARGET} INTERFACE)
