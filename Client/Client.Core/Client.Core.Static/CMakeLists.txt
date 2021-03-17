set(TARGET Client.Core.Static)
set(OUTPUT_NAME Client.Core.Static)

set(SOURCE_FILES
    Client.hpp)

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

source_group("Client"         FILES ${SOURCES})
source_group("Network"        FILES ${HEADERS})

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio)
target_link_libraries(${TARGET} PUBLIC Network.Public)

install(TARGETS ${TARGET}
		CONFIGURATIONS Debug
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
		CONFIGURATIONS Release
		RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)