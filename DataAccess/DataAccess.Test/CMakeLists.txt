set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

set(SOURCE_FILES
        DataAccessTest.cpp
        FileDBTest.cpp
        PostgreAdapterTest.cpp
        PostgreRepositoriesTest.cpp
        UsersAmountFinderTest.cpp 
        PostgreRepositoryManagerTest.cpp 
        PostgreRepositoriesContainerTest.cpp 
        PostgreQueryTest.cpp
        QueryBuilderTest.cpp
        AbstractRepositoryContainerTest.cpp
        )

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PRIVATE
        PostgreSQL::pq
        libpqxx::libpqxx
        Precompiled.Catch2.Main
        DataAccess.Postgre
        DataAccess.Debug
        Utility.Public)

target_include_directories(${TARGET} PRIVATE
        $<TARGET_PROPERTY:DataAccess.Postgre, SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
