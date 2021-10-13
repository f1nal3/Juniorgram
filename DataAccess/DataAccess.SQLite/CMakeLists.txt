set(TARGET DataAccess.SQLite)
set(OUTPUT_NAME DataAccess.SQLite)

set(SOURCE_FILES
        LiteAdapter.cpp
        LiteAdapter.hpp
        LiteTable.hpp
        LiteRepositoryContainer.hpp
        LiteRepositories.hpp
        LiteRepositories.cpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
        PUBLIC Qt::Sql DataAccess.Public Utility.Public)
