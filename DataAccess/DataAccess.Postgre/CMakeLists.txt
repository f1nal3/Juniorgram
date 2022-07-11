set(SUBTARGET Postgre)
set(TARGET  ${TARGET}.${SUBTARGET})

set(LIBRARY_OUTPUT_NAME ${TARGET})

set(HEADER_FILES
    PostgreAdapter.hpp
    PostgreRepositories.hpp
    UsersAmountFinder.hpp
)

set(HEADER_ONLY_FILES
    PostgreRepositoryContainer.hpp
    PostgreRepositoryManager.hpp
    PostgreTable.hpp
)

set(SOURCE_FILES
    PostgreAdapter.cpp
    PostgreRepositories.cpp
    UsersAmountFinder.cpp
)

add_library(${TARGET} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${HEADER_ONLY_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})



target_link_libraries(${TARGET}
                        PUBLIC PostgreSQL::pq libpqxx::pqxx
                        PRIVATE Utility.Public DataAccess.Public Network.Public )

target_include_directories(${TARGET}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)
