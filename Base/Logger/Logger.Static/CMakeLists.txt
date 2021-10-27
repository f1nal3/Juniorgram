set(TARGET Base.Logger.Static)
set(OUTPUT_NAME Base.Logger.Static)

set(SOURCE_FILES
        FileLogger.hpp
        FileLogger.cpp)

find_package(Threads REQUIRED)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})

target_link_libraries(${TARGET} PUBLIC
        Base.Logger.Public
        Utility.Public
        Threads::Threads)
