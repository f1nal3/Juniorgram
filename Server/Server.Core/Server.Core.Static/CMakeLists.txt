set(TARGET Server.Core.Static)
set(OUTPUT_NAME Server.Core.Static)

set(SOURCE_FILES
    Server.hpp
	Server.cpp
	ArgParser.hpp)

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

add_library(${TARGET} STATIC ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_include_directories(${TARGET}
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..)


target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} asio::asio yas::yas DataAccess.Postgre)
target_link_libraries(${TARGET} PUBLIC ${EXTRA_LIBS} argparse::argparse)
target_link_libraries(${TARGET} PUBLIC Network.Public DataAccess.Public Utility.Public Base.Logger.Static)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)
