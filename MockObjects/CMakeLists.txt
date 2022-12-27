project(MockObjects LANGUAGES CXX)

set(TARGET MockObjects)
set(FOLDER MockObjects)

add_subdirectory(Objects)

add_library(${TARGET} INTERFACE)
