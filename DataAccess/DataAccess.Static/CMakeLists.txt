set(TARGET DataAccess.Static)
set(OUTPUT_NAME DataAccess.Static)

set(QMAKE_CXXFLAGS  ${QMAKE_CXXFLAGS}"-fno-sized-deallocation")

set(SOURCE_FILES
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepository.hpp
    PostgreRepository.cpp
    PostgreTable.hpp
    #LiteAdapter.hpp
    #LiteAdapter.cpp
    #LiteTable.hpp
    RepositoryUnits.hpp
    RepositoryUnits.cpp
    SQLStatements.hpp
    QueryBuilder.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

#target_link_libraries(${TARGET}
#    PUBLIC Qt5::Sql)

target_link_libraries(${TARGET}
    PUBLIC CONAN_PKG::libpq CONAN_PKG::libpqxx)

target_link_libraries(${TARGET}
    PUBLIC Utility.Public DataAccess.Public Network.Public)
