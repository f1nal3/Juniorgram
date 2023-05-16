project(Utility LANGUAGES CXX)

set(TARGET Utility)
set(FOLDER Service.Core/Utility)

add_subdirectory(Utility.Public)
add_subdirectory(Utility.Test)



add_library(${TARGET} INTERFACE)
