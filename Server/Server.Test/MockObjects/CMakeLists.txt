set(TARGET MockObjects)
set(OUTPUT_NAME ${TARGET}.Objects)

set(HEADER_FILES
	Objects/MockDatabase.hpp
	Objects/MockQuery.hpp
	Objects/MockRepository.hpp
	Objects/MockClient.hpp
	)
	
set(SOURCE_FILES
	Objects/MockClient.cpp
	Objects/MockDatabase.cpp
	Objects/MockRepository.cpp
	)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Objects)

target_link_libraries(${TARGET} INTERFACE Server.Test)
