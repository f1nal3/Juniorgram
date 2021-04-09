cmake_minimum_required(VERSION 3.15.0)

set(TARGET DataAccess.Debug)
set(OUTPUT_NAME DataAccess.Debug)

set(SOURCES
	./FileDB.cpp
	./FileRepository.cpp)
	
set(HEADERS
	./FileDB.hpp
	./FileRepository.hpp)

add_library(${TARGET} STATIC ${SOURCES} ${HEADERS})

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

target_link_libraries(${TARGET} PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(${TARGET} PUBLIC
    Utility.Public
    DataAccess.Public)

