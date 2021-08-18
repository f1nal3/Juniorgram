set(TARGET Base.Static)
set(OUTPUT_NAME Base.Static)

set(SOURCE_FILES
    FileLogger.hpp
    FileLogger.cpp)

set(CMAKE_CXX_FLAGS -pthread)

find_package(Threads REQUIRED)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${TARGET} PUBLIC
    Base.Public
    Utility.Public
    Threads::Threads)
