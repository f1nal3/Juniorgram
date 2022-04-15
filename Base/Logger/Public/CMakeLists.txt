set(TARGET Base.Logger.Public)
set(OUTPUT_NAME Juniorgram.Base.Logger.Public)

set(SOURCE_FILES
        Include/Logger/ILogger.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)
