project(Test LANGUAGES CXX)

set(TARGET Test)
set(FOLDER Test)

add_subdirectory(Precompiled)

add_library(${TARGET} INTERFACE)
