set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Serialize.hpp
    Serialize.cpp
    SafePriorityQueue.hpp
    Task.hpp
    TaskManager.hpp
    Cryptography.cpp)


# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

<<<<<<< HEAD
target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core CONAN_PKG::cryptopp)
=======
target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core)
>>>>>>> cmake fixes

target_compile_options(${TARGET} PRIVATE "-fPIC")
