cmake_minimum_required(VERSION 3.17.0)

project(Juniorgram)

# Global settings. Should probably be moved to a separate file later.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CXX_EXTENSIONS off)

# For those VSCode users
set(EXPORT_COMPILE_COMMANDS TRUE)

# Set warnings level higher and force treating them as errors.
# GCC TBD
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/master/conan.cmake"
                 "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

if(WIN32)
conan_cmake_run(CONANFILE conanfile_win.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)
elseif(APPLE)
conan_cmake_run(CONANFILE conanfile_mac.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)
else()
conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)
endif()

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
if (Qt5_FOUND)
     message( "Qt5 = ${Qt5_FOUND}" )
else()
     message(FATAL_ERROR "Qt not found!")
endif()

enable_testing()

add_subdirectory(Client)
add_subdirectory(DataAccess)
add_subdirectory(Network)
add_subdirectory(Server)
add_subdirectory(Utility)

