project(Kernel.Core LANGUAGES CXX)

add_subdirectory(Kernel.Core.Static)

set(TARGET Kernel.Core)

add_library(${TARGET} INTERFACE)
