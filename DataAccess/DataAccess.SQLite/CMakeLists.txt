set(TARGET DataAccess.SQLite)
set(OUTPUT_NAME DataAccess.SQLite)

set(QMAKE_CXXFLAGS ${QMAKE_CXXFLAGS}"-fno-sized-deallocation")

set(SOURCE_FILES
    LiteAdapter.cpp
    LiteAdapter.hpp
    LiteTable.hpp
	LiteRepositoryContainer.hpp
	ClientRepositories.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET}
    PUBLIC Qt5::Sql)

target_link_libraries(${TARGET}
    PUBLIC DataAccess.Public Utility.Public)
