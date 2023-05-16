set(TARGET Test)
set(FOLDER Test)

add_subdirectory(Precompiled)
add_subdirectory(Crypto.Test)
add_subdirectory(DataAccess.Test)
add_subdirectory(Kernel.Test)
add_subdirectory(Logger.Test)
add_subdirectory(Network.Test)
add_subdirectory(Utility.Test)

add_library(${TARGET} INTERFACE)
