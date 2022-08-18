set(TARGET Server.Tests)
set(OUTPUT_NAME Server.Tests)

set(SOURCE_FILES
        ArgumentParserTests.cpp
		ServerFeaturesOfMessagesTests.cpp
		ServerDefaultMessagesTests.cpp
		ServerChannelMessagesTests.cpp
		ServerAutorizationMessagesTests.cpp
		ServerWorkflowTests.cpp
		)

include_directories(TestUtility)
include_directories(MockObjects)

add_executable(${TARGET} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET}
PUBLIC 
    Precompiled.Catch2.Main
    Network.Public 
    Server.Core.Static
    DataAccess.Debug
    yas::yas
    asio::asio
	PostgreSQL::pq 
    libpqxx::libpqxx    
    Utility.Public
    Models.Public
    Base.Crypto.Static
	MockObjects
	)

target_include_directories(${TARGET} PRIVATE
	$<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)
target_include_directories(${TARGET} PUBLIC 
	$<TARGET_PROPERTY:Client.Core,SOURCE_DIR>/..)		
target_include_directories(${TARGET} PUBLIC 
	$<TARGET_PROPERTY:TestUtility,SOURCE_DIR>/..)	
	
target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
