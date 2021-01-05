cmake_minimum_required(VERSION 3.10.0)

project(Juniorgram)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

add_subdirectory(client)
add_subdirectory(server)
	
