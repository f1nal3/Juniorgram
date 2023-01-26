set(TARGET Client.Core.Static)
set(OUTPUT_NAME Client.Core.Static)

set(SOURCE_FILES
        Client.hpp
        Client.cpp
        ServerInfo.hpp)

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif ()

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET}
        PRIVATE ${EXTRA_LIBS} 
                asio::asio 
                yas::yas
        PUBLIC Network.Public 
                Utility.Public
                Base.Crypto.Static 
                Base.Logger.Static
                Models.Public)

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PRIVATE $<TARGET_PROPERTY:Base.Crypto.Static,SOURCE_DIR>/..)
