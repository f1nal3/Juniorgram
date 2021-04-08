set(TARGET Client.Core.Static)
set(OUTPUT_NAME Client.Core.Static)


set(SOURCE_FILES
    Client.hpp
	Client.cpp)

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio)
target_link_libraries(${TARGET} PUBLIC Network.Public
									   DataAccess.Public)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)