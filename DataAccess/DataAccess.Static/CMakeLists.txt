set(TARGET DataAccess.Static)
set(OUTPUT_NAME DataAccess.Static)

set(QMAKE_CXXFLAGS  ${QMAKE_CXXFLAGS}"-fno-sized-deallocation")

set(SOURCE_FILES
    PostgreAdapter.hpp
    PostgreAdapter.cpp
    PostgreRepository.hpp
    PostgreRepository.cpp
	SQLCipherAdapter.hpp
	SQLCipherAdapter.cpp
	SQLCipherRepository.hpp
	SQLCipherRepository.cpp
	SQLCipherTable.hpp
    PostgreTable.hpp
    LiteAdapter.hpp
    LiteAdapter.cpp
    LiteTable.hpp
    RepositoryUnits.hpp
    RepositoryUnits.cpp
    SQLStatements.hpp
    QueryBuilder.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)
	
target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} PUBLIC
	PUBLIC Qt5::Sql
    Utility.Static
	Network.Static
	CONAN_PKG::libpq
    CONAN_PKG::libpqxx
    CONAN_PKG::sqlcipher
    CONAN_PKG::cryptopp
    CONAN_PKG::nlohmann_json)

target_link_libraries(${TARGET}
    PUBLIC Utility.Public DataAccess.Public)
