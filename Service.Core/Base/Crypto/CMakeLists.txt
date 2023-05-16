project(Crypto LANGUAGES CXX)

add_subdirectory(Crypto.Static)
add_subdirectory(Crypto.Test)

set(TARGET Crypto)

add_library(${TARGET} INTERFACE)