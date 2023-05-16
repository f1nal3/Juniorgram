project(Network LANGUAGES CXX)

set(TARGET Network)
set(FOLDER Service.Core/Network)

add_subdirectory(Public)
add_subdirectory(Network.Test)

set(TARGET Network)

add_library(${TARGET} INTERFACE)
