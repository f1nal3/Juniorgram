set(SUBTARGET Postgre)
set(TARGET  ${TARGET}.${SUBTARGET})

set(LIBRARY_OUTPUT_NAME ${TARGET})

set(HEADERS
    PostgreAdapter.hpp
    PostgreRepositories.hpp
    UsersAmountFinder.hpp
    PostgreModel.hpp
    Models.hpp)

set(HEADERS_ONLY
    PostgreRepositoryContainer.hpp
    PostgreRepositoryManager.hpp
    PostgreQuery.hpp)

set(SOURCES
    PostgreAdapter.cpp
    PostgreRepositories.cpp
    UsersAmountFinder.cpp)

set(SOURCE_FILES ${SOURCES} ${HEADERS} ${HEADERS_ONLY})

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET}
        PUBLIC PostgreSQL::pq libpqxx::pqxx
        PUBLIC Utility.Public DataAccess.Public Network.Public Models.Public)

target_include_directories(${TARGET}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
