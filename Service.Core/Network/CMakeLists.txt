set(TARGET Network)
set(FOLDER Service.Core/Network)

add_subdirectory(Public)
add_subdirectory(Network.Test)


add_library(${TARGET} INTERFACE)
