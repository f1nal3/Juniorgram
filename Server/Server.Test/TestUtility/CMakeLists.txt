set(TARGET Utility.test)
set(OUTPUT_NAME Utility.test)

set(SOURCE_FILES 
	TestUtility.hpp
	)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
	
target_link_libraries(${TARGET} 
PUBLIC
	Network.Public 
    Server.Core.Static
    yas::yas
    Utility.Public
	Models.Public
	Base.Crypto.Static
	Client.Core.Static
    )
	
set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)
