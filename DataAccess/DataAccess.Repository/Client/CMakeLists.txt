set(TARGET DataAccess.Repository.Client)
set(OUTPUT_NAME DataAccess.Repository.Client)

set(SOURCE_FILES
    Repositories.hpp
    ClientRepositoryContainer.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET}
    PUBLIC DataAccess.Repository.Public DataAccess.SQLite)
