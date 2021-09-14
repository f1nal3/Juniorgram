set(TARGET DataAccess.Postgre)
set(OUTPUT_NAME DataAccess.Postgre)

set(SOURCE_FILES
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepository.hpp
    PostgreRepository.cpp
	PostgreServerRepository.hpp
    PostgreTable.hpp
    SQLStatements.hpp
    QueryBuilder.hpp
    DataBaseOptions.hpp
    UsersAmountFinder.hpp
    UsersAmountFinder.cpp
	ServerRepositories.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
    PUBLIC CONAN_PKG::libpq CONAN_PKG::libpqxx)

target_link_libraries(${TARGET}
    PUBLIC Utility.Public DataAccess.Public Network.Public)
