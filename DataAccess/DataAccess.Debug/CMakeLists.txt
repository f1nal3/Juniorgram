cmake_minimum_required(VERSION 3.15.0)

project(FileDB)

set(TARGET DataAccess.Debug)
set(OUTPUT_NAME DataAccess.Debug)

set(SOURCES
	./mainFileDB.cpp
	./FileDB.cpp)
	
set(HEADERS
	./FileDB.hpp)

add_executable(FileDB ${SOURCES} ${HEADERS})

include(FetchContent)

#	Note: The repository https://github.com/nlohmann/json download size is huge.
#		It contains all the dataset used for the benchmarks. You might want to depend on a smaller repository.
#		For instance, you might want to replace the URL 
#		above by https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Utility.Public
    DataAccess.Public)

