project(Base LANGUAGES CXX)

add_subdirectory(Logger)
add_subdirectory(Crypto)

set(TARGET Base)

add_library(${TARGET} INTERFACE)
