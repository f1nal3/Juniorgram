project(Kernel LANGUAGES CXX)

set(TARGET Kernel)
set(FOLDER Kernel)

add_subdirectory(Kernel.Exec)
add_subdirectory(Kernel.Test)
add_subdirectory(Kernel.Core)

add_library(${TARGET} INTERFACE)
