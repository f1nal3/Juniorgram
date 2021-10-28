project(Utility LANGUAGES CXX)

set(TARGET Utility)
set(FOLDER "Utility")

add_subdirectory(Utility.Public)
add_subdirectory(Utility.Test)



add_library(${TARGET} INTERFACE)
