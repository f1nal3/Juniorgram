set(TARGET Base)
set(FOLDER Service.Core/Base)

add_subdirectory(Logger)
add_subdirectory(Crypto)
add_subdirectory(Models)

add_library(${TARGET} INTERFACE)
