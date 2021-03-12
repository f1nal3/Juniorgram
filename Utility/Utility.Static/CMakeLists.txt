set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Serialize.hpp
    Serialize.cpp)

# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} 
    PRIVATE CONAN_PKG::Qt)

target_compile_options(${TARGET} PRIVATE "-fPIC")
