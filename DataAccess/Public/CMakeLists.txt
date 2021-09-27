set(TARGET DataAccess.Public)
set(OUTPUT_NAME Juniorgram.DataAccess.Public)

set(SOURCE_FILES
        Include/DataAccess/IAdapter.hpp
        Include/DataAccess/IRepository.hpp
        Include/DataAccess/QueryBuilder.hpp
        Include/DataAccess/SQLStatements.hpp
        Include/DataAccess/IMasterRepository.hpp
        Include/DataAccess/IClientRepositories.hpp
        Include/DataAccess/IServerRepositories.hpp
        Include/DataAccess/AbstarctRepositoryContainer.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
