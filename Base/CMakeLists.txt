project(Base LANGUAGES CXX)

add_subdirectory(Base.Public)
add_subdirectory(Base.Static)
add_subdirectory(Base.Test)

set(TARGET Base)

add_library(${TARGET} INTERFACE)
