set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    CompressionHandler.hpp
    Cryptography.cpp
    EncryptionHandler.hpp
    Handler.hpp
    JsonSerializer.hpp
    JsonSerializer.cpp
    SafePriorityQueue.hpp
    SerializationHandler.hpp
    Task.hpp
    TaskManager.hpp
    YasSerializer.hpp)


# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core Network.Public CONAN_PKG::cryptopp CONAN_PKG::asio CONAN_PKG::yas)

target_compile_options(${TARGET} PRIVATE "-fPIC")
