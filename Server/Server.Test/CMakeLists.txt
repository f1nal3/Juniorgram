project(Server LANGUAGES CXX)

set(TARGET Server.Test)
set(FOLDER Server.Test)

add_subdirectory(Tests)
add_subdirectory(TestUtility)
add_subdirectory(MockObjects)

add_library(${TARGET} INTERFACE)
