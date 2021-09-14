set(TARGET DataAccess.Repository.Server)
set(OUTPUT_NAME DataAccess.Repository.Server)

set(SOURCE_FILES
    Repositories.hpp
    ServerRepositoryContainer.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
    PUBLIC DataAccess.Repository.Public DataAccess.Postgre)
