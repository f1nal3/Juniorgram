set(TARGET Base.Models.Public)
set(OUTPUT_NAME ${PROJECT}.Base.Models.Public)

set(SOURCE_FILES
        Include/Models/Primitives.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
