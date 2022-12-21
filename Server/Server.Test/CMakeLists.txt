set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
        ArgumentParserTests.cpp
		ServerFeaturesOfMessagesTests.cpp
		ServerDefaultMessagesTests.cpp
		ServerChannelMessagesTests.cpp
		ServerAutorizationMessagesTests.cpp
		ServerWorkflowTests.cpp
		
		MockObjects/MockClient.cpp
		MockObjects/MockDatabase.cpp
		MockObjects/MockRepository.cpp)

set(HEADER_FILES
	MockObjects/MockQuery.hpp
	MockObjects/MockClient.hpp
	MockObjects/MockDatabase.hpp
	MockObjects/MockRepository.hpp
	)

add_executable(${TARGET} ${SOURCE_FILES} ${HEADER_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

include_directories(TestUtility)

target_link_libraries(${TARGET}
PUBLIC 
    Precompiled.Catch2.Main 
    Server.Core.Static
	Client.Core.Static
    DataAccess.Debug
	PostgreSQL::pq 
    libpqxx::libpqxx    
	asio::asio
	)

 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
