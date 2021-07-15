set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
    Cryptography.hpp
    Cryptography.cpp
    JsonSerializer.hpp
    JsonSerializer.cpp
    SafePriorityQueue.hpp
    TaskManager.hpp
	MACAddress.hpp
	MACAddress.cpp
	ClientPayload.hpp
	ClientPayload.cpp
	TokenHolder.hpp
	TokenHolder.cpp
	YasSerializer.hpp
	YasSerializer.cpp
	Handler.hpp
	Handler.cpp
	SerializationHandler.hpp
	SerializationHandler.cpp
	EncryptionHandler.hpp
	EncryptionHandler.cpp
	CompressionHandler.hpp
	CompressionHandler.cpp)
	
# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} Qt5::Widgets Qt5::Core Network.Public DataAccess.Static Utility.Public CONAN_PKG::nlohmann_json CONAN_PKG::cryptopp CONAN_PKG::asio CONAN_PKG::yas CONAN_PKG::lz4)

target_compile_options(${TARGET} PRIVATE "-fPIC")
