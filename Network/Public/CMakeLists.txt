set(TARGET Network.Public)
set(OUTPUT_NAME Juniorgram.Network.Public)

set(SOURCE_FILES
    Include/Network/SafeQueue.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_include_directories(${TARGET} INTERFACE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} INTERFACE Utility.Public Utility.Static CONAN_PKG::yas)
