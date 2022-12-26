project(Objects LANGUAGES CXX)

add_subdirectory(TestObjectsFiles)

set(TARGET Objects)

add_library(${TARGET} INTERFACE)
