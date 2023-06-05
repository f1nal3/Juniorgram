
# Improves project structure representation on MSVS
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Turn this on to get more info during CMake configuration
set(CMAKE_VERBOSE_MAKEFILE OFF)

# For those VSCode users
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Destination for installing test executables
set(TEST_INSTALL_DIR ${CMAKE_BINARY_DIR}/bin/test)
