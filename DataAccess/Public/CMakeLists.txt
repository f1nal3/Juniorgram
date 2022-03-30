set(TARGET DataAccess.Public)
set(OUTPUT_NAME ${PROJECT}.DataAccess.Public)

set(SOURCE_FILES
        Include/DataAccess/AbstractRepositoryContainer.hpp
        Include/DataAccess/IAdapter.hpp
        Include/DataAccess/IClientRepositories.hpp
        Include/DataAccess/IMasterRepository.hpp
        Include/DataAccess/IRepository.hpp
        Include/DataAccess/IServerRepositories.hpp
        Include/DataAccess/SQLStatements.hpp
        Include/DataAccess/QueryBuilder.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
