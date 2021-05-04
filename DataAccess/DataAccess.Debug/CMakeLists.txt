cmake_minimum_required(VERSION 3.15.0)

set(TARGET DataAccess.Debug)
set(OUTPUT_NAME DataAccess.Debug)

set(SOURCE_FILES
	./FileDB.hpp
	./FileDB.cpp
	./FileRepository.hpp
	./FileRepository.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

target_link_libraries(${TARGET} PUBLIC
    Utility.Public
    DataAccess.Public
	Network.Public
	nlohmann_json::nlohmann_json)

