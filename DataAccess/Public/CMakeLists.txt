set(TARGET DataAccess.Public)
set(OUTPUT_NAME Juniorgram.DataAccess.Public)

set(SOURCE_FILES
    Include/DataAccess/IDB.hpp
    Include/DataAccess/IRepository.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})
target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
