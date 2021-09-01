project(Crypto LANGUAGES CXX)

add_subdirectory(Crypto.Static)

set(TARGET Crypto)

add_library(${TARGET} INTERFACE)