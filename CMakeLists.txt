cmake_minimum_required(VERSION 3.17.0)

set(PROJECT Juniorgram)
project(Juniorgram)

# Global settings. Should probably be moved to a separate file later.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CXX_EXTENSIONS off)

# Oprions
option(ENABLE_CCACHE        "Enable the usage of Ccache."           ON)
option(ENABLE_WARNINGS      "Enable to add warnings to a target."   ON)


# Cmake modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
# CMake properies
include(CMakeConfig)
# Conan and dependencies
include(ConanModule)

# Warnings toggle should be implemented here
if(ENABLE_WARNINGS)
    include(Warnings)
    set_warnings()
endif()

# CCache
if (ENABLE_CCACHE)
    include(Ccache)
endif()

if (COVERAGE_MODE)
    include(Coverage)
endif()

enable_testing()

add_subdirectory(Test)
add_subdirectory(Base)
add_subdirectory(Client)
add_subdirectory(DataAccess)
add_subdirectory(Network)
add_subdirectory(Server)
add_subdirectory(Utility)
