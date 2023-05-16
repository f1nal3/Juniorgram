set(TARGET Kernel.Test)
set(OUTPUT_NAME Kernel.Test)

set(SOURCE_FILES
		ArgParserTests.cpp
		ServerMessageTests.cpp
		ServerDefaultTests.cpp
		ServerChannelTests.cpp
		ServerWorkflowTests.cpp
		ServerAutorizationTests.cpp)

set(TEST_SOURCE_FILES
		TestUtils/WaitForTime.hpp
		TestUtils/TestUtility.hpp
		TestUtils/MessageFiller.hpp
		TestUtils/MessageFiller.cpp
		TestUtils/MockMessagesOptions.hpp 
		TestUtils/MockClient.cpp
		TestUtils/MockClient.hpp
		TestUtils/TestServerInfo.hpp)

add_executable(${TARGET} ${SOURCE_FILES} ${TEST_SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIB}
				argparse::argparse 
				asio::asio
		PUBLIC  Precompiled.Catch2.Main 
				Kernel.Core
				DataAccess.Mock)

 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
