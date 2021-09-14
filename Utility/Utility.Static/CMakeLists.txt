set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    JsonSerializer.hpp
    JsonSerializer.cpp)

# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core Network.Public CONAN_PKG::asio CONAN_PKG::yas)
