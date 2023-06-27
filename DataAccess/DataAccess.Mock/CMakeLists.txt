set(TARGET DataAccess.Mock)
set(OUTPUT_NAME DataAccess.Mock)

set(SOURCE_FILES
    MockQuery.hpp
	MockTables.hpp
	MockDatabase.hpp
	MockDatabase.cpp
	MockQueryBuilder.hpp
	MockRepositories.hpp
	MockRepositories.cpp
	MockDatabaseOptions.hpp
	MockRepositoryInstance.hpp
	MockRepositoryStorage.hpp
	MockRepositoryManager.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
PUBLIC
	Base.Crypto.Static
    Kernel.Core.Static
    DataAccess.Debug)
	
 target_include_directories(${TARGET} PRIVATE
	 $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)
	 
set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})
