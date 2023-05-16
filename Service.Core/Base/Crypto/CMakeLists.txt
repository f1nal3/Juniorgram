set(TARGET Crypto)

add_subdirectory(Crypto.Static)
add_subdirectory(Crypto.Test)

add_library(${TARGET} INTERFACE)