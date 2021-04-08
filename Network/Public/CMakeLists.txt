set(TARGET Network.Public)
set(OUTPUT_NAME Juniorgram.Network.Public)

set(SOURCE_FILES
    Include/Network/Connection.hpp
    Include/Network/Message.hpp
    Include/Network/SafeQueue.hpp
    Include/Network/Primitives.hpp
    Include/Network/MessageWrapper.hpp)

add_library(${TARGET} INTERFACE)

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(${TARGET} INTERFACE Utility.Public)
