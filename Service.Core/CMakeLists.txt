project(Service.Core LANGUAGES CXX)

set(TARGET Service.Core)
set(FOLDER Service.Core)

add_subdirectory(Base)
add_subdirectory(DataAccess)
add_subdirectory(Network)
add_subdirectory(Kernel)
add_subdirectory(Utility)

add_library(${TARGET} INTERFACE)
