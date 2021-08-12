set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Cryptography.hpp
    Cryptography.cpp
 
   )

# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Network.Public CONAN_PKG::cryptopp CONAN_PKG::asio CONAN_PKG::yas)

