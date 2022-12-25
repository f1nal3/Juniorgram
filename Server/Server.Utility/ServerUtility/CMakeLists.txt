set(TARGET Server.Utility.Test)
set(OUTPUT_NAME Server.Utility.Test)

set(SOURCE_FILES 
	TestUtility.hpp
	)

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
 target_include_directories(${TARGET} PRIVATE
	  $<TARGET_PROPERTY:MockObjectsFiles,SOURCE_DIR>/..)
	 
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
