project(Kernel LANGUAGES CXX)

set(TARGET ${PROJECT_NAME})
set(FOLDER Service.Core/${PROJECT_NAME})

add_subdirectory(Kernel.Core)
add_subdirectory(Kernel.Test)

add_library(${TARGET} INTERFACE)
