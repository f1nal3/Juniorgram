set(TARGET DataAccess.Postgre)
set(OUTPUT_NAME DataAccess.Postgre)

set(SOURCE_FILES
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepositories.hpp
    PostgreRepositories.cpp
	PostgreRepositoryContainer.hpp
    PostgreRepositoryManager.hpp
    PostgreTable.hpp
    UsersAmountFinder.hpp
    UsersAmountFinder.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET}
        PUBLIC PostgreSQL::pq libpqxx::libpqxx
        PUBLIC Utility.Public DataAccess.Public Network.Public Server.Core.Static)
