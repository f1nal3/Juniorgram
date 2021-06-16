set(TARGET Server.Core.Static)
set(OUTPUT_NAME Server.Core.Static)

set(SOURCE_FILES
    Server.hpp
	Server.cpp
	ArgumentParser.hpp
	KeysValidator.hpp)

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)
	        
target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio CONAN_PKG::yas DataAccess.Static)
target_link_libraries(${TARGET} PUBLIC Network.Public DataAccess.Public Utility.Public)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)
