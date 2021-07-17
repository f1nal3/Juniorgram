set(TARGET Network.Static)
set(OUTPUT_NAME Network.Static)

set(SOURCE_FILES
    Connection.hpp
    Connection.cpp
	Message.hpp
	Message.cpp
	Primitives.hpp
	Primitives.cpp
	SafeQueue.hpp
	SafeQueue.cpp)
	
# Add a library with the above sources
add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)
	
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} Network.Public DataAccess.Static Utility.Public Utility.Static CONAN_PKG::cryptopp CONAN_PKG::asio)

target_compile_options(${TARGET} PRIVATE "-fPIC")
