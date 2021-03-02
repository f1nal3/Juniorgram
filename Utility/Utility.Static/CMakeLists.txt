set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Serialize.hpp
    Serialize.cpp)

# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT})

target_compile_options(${TARGET} PRIVATE "-fPIC")
