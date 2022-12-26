set(TARGET TestObjectsFiles)
set(OUTPUT_NAME TestObjectsFiles)

set(SOURCE_FILES
    TestQuery.hpp
	TestClient.cpp
	TestClient.hpp
	TestDatabase.hpp
	TestDatabase.cpp
	TestRepositories.hpp
	TestRepositories.cpp
	TestRepositoryManager.hpp
	TestIRepositoryManager.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
PUBLIC 
    Server.Core.Static
	Client.Core.Static
    DataAccess.Debug
	PostgreSQL::pq 
    libpqxx::libpqxx    
	asio::asio
	)
 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)
	 
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
