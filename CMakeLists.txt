cmake_minimum_required(VERSION 3.17.0)

set(PROJECT Juniorgram)
project(Juniorgram)

# Global settings. Should probably be moved to a separate file later.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CXX_EXTENSIONS off)

# For those VSCode users
set(EXPORT_COMPILE_COMMANDS TRUE)

# Oprions
option(ENABLE_CCACHE        "Enable the usage of Ccache."           ON)
option(ENABLE_WARNINGS      "Enable to add warnings to a target."   ON)


# Cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
# CMake properies
include(CMakeConfig)
# conan
include(ConanModule)
# warnings
if(ENABLE_WARNINGS)
    include(Warnings)
    set_warnings()
endif()
# ccache
if (ENABLE_CCACHE)
    include(Ccache)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui Sql REQUIRED HINTS "/opt/Qt/5.15.2/gcc_64/lib/cmake/Qt5/")

enable_testing()

add_subdirectory(Test)
add_subdirectory(Base)
add_subdirectory(Client)
add_subdirectory(DataAccess)
add_subdirectory(Network)
add_subdirectory(Server)
add_subdirectory(Utility)
