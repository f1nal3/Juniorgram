set(TARGET TestUtility)
set(OUTPUT_NAME TestUtility)

set(SOURCE_FILES 
	TestUtility.hpp
	)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PUBLIC 
	$<TARGET_PROPERTY:Client.Core,SOURCE_DIR>/..)

target_link_libraries(${TARGET} 
PUBLIC
	Network.Public 
    Server.Core.Static
    DataAccess.Debug
    yas::yas
    asio::asio   
    Utility.Public
	Models.Public
	Base.Crypto.Static
    )

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})
