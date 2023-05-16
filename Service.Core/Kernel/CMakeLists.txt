project(Kernel LANGUAGES CXX)

set(TARGET ${PROJECT_NAME})
set(FOLDER ${PROJECT_NAME})

add_subdirectory(Kernel.Core)
add_subdirectory(Kernel.Exec)
add_subdirectory(Kernel.Test)

add_library(${TARGET} INTERFACE)
