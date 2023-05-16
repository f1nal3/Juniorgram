set(TARGET Kernel)
set(FOLDER Service.Core/${TARGET})

add_subdirectory(Kernel.Core)
add_subdirectory(Kernel.Test)

add_library(${TARGET} INTERFACE)
