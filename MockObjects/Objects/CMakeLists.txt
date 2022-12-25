project(Objects LANGUAGES CXX)

add_subdirectory(MockObjectsFiles)

set(TARGET Objects)

add_library(${TARGET} INTERFACE)
