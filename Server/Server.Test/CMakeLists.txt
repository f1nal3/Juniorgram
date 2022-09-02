set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
        ArgumentParserTests.cpp
		ServerFeaturesOfMessagesTests.cpp
		ServerDefaultMessagesTests.cpp
		ServerChannelMessagesTests.cpp
		ServerAutorizationMessagesTests.cpp
		ServerWorkflowTests.cpp
		)

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

include_directories(TestUtility)
add_subdirectory(MockObjects)

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
	Client.Core.Static
    MockObjects
	)

 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
