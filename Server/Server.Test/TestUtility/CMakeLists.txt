set(TARGET TestUtility)

set(OUTPUT_NAME TestUtility)

set(SOURCE_FILES
	TestUtility.hpp
	)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} 
PUBLIC
	Network.Public 
    Server.Core.Static
    DataAccess.Debug
    yas::yas
    asio::asio
	PostgreSQL::pq
    libpqxx::libpqxx    
    Utility.Public
	Base.Crypto.Static
    )

target_include_directories(${TARGET} PRIVATE
	$<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)

target_include_directories(${TARGET} PUBLIC 
	$<TARGET_PROPERTY:Client.Core,SOURCE_DIR>/..)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)