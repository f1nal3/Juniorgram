set(TARGET Client.TestObject)
set(OUTPUT_NAME Client.TestObject)

set(SOURCE_FILES
	TestClient.hpp
	TestClient.cpp
	TestServerInfo.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
PUBLIC 
    Client.Core.Static)
	
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
