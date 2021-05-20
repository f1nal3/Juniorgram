set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Cryptography.cpp
    JsonSerializer.hpp
    JsonSerializer.cpp
    SafePriorityQueue.hpp
    Task.hpp
    TaskManager.hpp)


# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core CONAN_PKG::cryptopp)

target_compile_options(${TARGET} PRIVATE "-fPIC")
