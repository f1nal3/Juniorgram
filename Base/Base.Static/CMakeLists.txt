set(TARGET Base.Static)
set(OUTPUT_NAME Base.Static)

set(SOURCE_FILES
    FileLogger.hpp
    FileLogger.cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TARGET} PUBLIC
    Base.Public
    Utility.Public)
