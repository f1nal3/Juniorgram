project(Client.Core LANGUAGES CXX)

add_subdirectory(Client.Core.Static)
add_subdirectory(Client.Core.Test)

set(TARGET Client.Core)

add_library(TARGET INTERFACE)
set(SOURCES
    ./app.cpp
    ./mainClient.cpp)
