set(TARGET Server.Test)
set(OUTPUT_NAME Server.Test)

set(SOURCE_FILES
		ArgParserTests.cpp
		ServerMessageTests.cpp
		ServerDefaultTests.cpp
		ServerChannelTests.cpp
		ServerWorkflowTests.cpp
		ServerAutorizationTests.cpp
        ServerSettingsTests.cpp
        ServerSettingsManagerTests.cpp)

set(TEST_SOURCE_FILES
		Utilities/WaitForTime.hpp
		Utilities/TestUtility.hpp
		Utilities/MessageFiller.hpp
		Utilities/MessageFiller.cpp
		Utilities/MockMessagesOptions.hpp)

add_executable(${TARGET} ${SOURCE_FILES} ${TEST_SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIB}
				argparse::argparse 
				asio::asio
		PUBLIC  Precompiled.Catch2.Main 
				Server.Core.Static
				DataAccess.Mock
				Client.TestObject)

 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)
 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:Client.Core,SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})

install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${TEST_INSTALL_DIR})
