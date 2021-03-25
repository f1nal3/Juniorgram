set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Serialize.hpp
    Serialize.cpp
    SafePriorityQueue.h
    Task.h
    TaskManager.h)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core)

target_compile_options(${TARGET} PRIVATE "-fPIC")
