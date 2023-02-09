set(TARGET Client.Mock)
set(OUTPUT_NAME Client.Mock)

set(SOURCE_FILES
	MockClient.hpp
	MockClient.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
PUBLIC 
    Client.Core.Static)
	
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
