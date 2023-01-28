set(TARGET MockObjectsFiles)
set(OUTPUT_NAME MockObjectsFiles)

set(SOURCE_FILES
    MockQuery.hpp
	MockClient.cpp
	MockClient.hpp
	MockDatabase.hpp
	MockDatabase.cpp
	MessageFiller.hpp
	MessageFiller.cpp
	TestMessagesOptions.hpp
	MockRepositories.hpp
	MockRepositories.cpp
	MockRepositoryManager.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
PUBLIC 
    Server.Core.Static
	Client.Core.Static
    DataAccess.Debug
	PostgreSQL::pq 
    libpqxx::libpqxx    
	asio::asio)
	
 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)
	 
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
