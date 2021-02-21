cmake_minimum_required( VERSION 3.16 )
 
project(Seialize_lib VERSION 1.0)
 
set(SOURCES 
    /src/serialize.cpp)
 
set(HEADERS 
    /include/serialize.h)
 
# Add a library with the above sources
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(Utility::serialize ALIAS ${PROJECT_NAME})
 
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
)
