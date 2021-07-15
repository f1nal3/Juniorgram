set(TARGET Client.Core.Static)
set(OUTPUT_NAME Client.Core.Static)

set(SOURCE_FILES
        Client.hpp
        Client.cpp
        TokenHolder.hpp
        TokenHolder.cpp)

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif ()

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET}
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
		
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET}
        PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio CONAN_PKG::yas CONAN_PKG::lz4
        PUBLIC Utility.Static
        PUBLIC Network.Public
		PUBLIC DataAccess.Static)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)
