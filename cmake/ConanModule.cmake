# This is a CMake file that configures Conan and external dependencies
# This is the place to add any additional external packages in the future.


# Download conan.cmake file for future use
# https://github.com/conan-io/cmake-conan is the go-to website if you have questions.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.0/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

# Includes the downloaded file
include(${CMAKE_BINARY_DIR}/conan.cmake)

# Uses conan.cmake functionality to list the needed packages.
conan_cmake_configure(REQUIRES asio/1.18.1
                               cryptopp/8.5.0
                               expat/2.2.10
                               catch2/2.13.4
                               libtool/2.4.6
                               libpq/13.6
                               libpqxx/7.7.0
                               nlohmann_json/3.9.1
                               yas/7.0.5
                               qt/5.15.2
                    GENERATORS cmake_find_package_multi)

# This detects system settings such as platform, compiler, etc.
# Those can be overridden, see conan.cmake github for documentation.
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
                    REMOTE conancenter
                    BUILD missing
                    INSTALL_FOLDER ${CMAKE_BINARY_DIR}/Conan
                    SETTINGS ${settings})

# Append the folder with
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Conan)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# List of find_packages for each external library that we use.
# TODO: Qt has HINT due to legacy CI issues. Need to check if this persists.
find_package(Qt5 COMPONENTS Widgets Core Gui Sql REQUIRED HINTS "/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5/")
find_package(asio REQUIRED)
find_package(Catch2 REQUIRED)
find_package(cryptopp REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(yas REQUIRED)
