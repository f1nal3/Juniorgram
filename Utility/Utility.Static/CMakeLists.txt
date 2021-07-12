set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Cryptography.hpp
    Cryptography.cpp
    JsonSerializer.hpp
    JsonSerializer.cpp
    SafePriorityQueue.hpp
    TaskManager.hpp
    SymmetricCipher.cpp
	MACAddress.hpp
	MACAddress.cpp)
	
# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core Network.Public Utility.Public CONAN_PKG::cryptopp CONAN_PKG::asio CONAN_PKG::yas CONAN_PKG::lz4)

target_compile_options(${TARGET} PRIVATE "-fPIC")
