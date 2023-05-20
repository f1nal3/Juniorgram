set(TARGET Models.Public)
set(OUTPUT_NAME ${PROJECT}.Models.Public)

set(SOURCE_FILES
        Include/Models/Primitives.hpp
        Include/Models/UnifyedModel.hpp
        Include/Models/Models.hpp
)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
