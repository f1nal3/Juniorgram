set(TARGET DataAccess.Test)
set(OUTPUT_NAME DataAccess.Test)

set(SOURCE_FILES
        DataAccessTest.cpp
        FileDBTest.cpp
        RepositoryFunctionsTest.cpp
        TestPostgreRepository.hpp
        TestUsersAmountFinder.hpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC
        CONAN_PKG::libpq
        CONAN_PKG::libpqxx
        Precompiled.Catch2.Main
        DataAccess.Postgre
        DataAccess.Debug
        Utility.Public
        Utility.Static)

target_include_directories(${TARGET} PRIVATE
        $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)

target_include_directories(${TARGET} PRIVATE
        $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
