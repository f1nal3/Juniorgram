project(Base LANGUAGES CXX)

add_subdirectory(Logger)

set(TARGET Base)

add_library(${TARGET} INTERFACE)
