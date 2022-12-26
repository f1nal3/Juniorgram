project(TestObjects LANGUAGES CXX)

set(TARGET TestObjects)
set(FOLDER TestObjects)

add_subdirectory(Objects)

add_library(${TARGET} INTERFACE)
