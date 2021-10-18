set(TARGET Utility.Static)
set(OUTPUT_NAME Utility.Static)

set(SOURCE_FILES
        empty.cpp
        )

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET})
