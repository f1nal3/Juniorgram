set(TARGET Kernel.Core)
set(OUTPUT_NAME ${TARGET})

set(SOURCE_FILES
    Kernel.hpp
	Kernel.cpp
	ArgParser.hpp
    KernelBuilder.hpp)

if(WIN32)
	list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif()

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}
										argparse::argparse
										asio::asio 
										yas::yas  
										Network.Public  
										Utility.Public  
										Base.Logger.Static
                                PUBLIC	DataAccess.Postgre  
										DataAccess.Public)

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE 
								CXX FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
									 PRIVATE $<TARGET_PROPERTY:DataAccess.Postgre,SOURCE_DIR>/..
									 PRIVATE $<TARGET_PROPERTY:DataAccess.Public,SOURCE_DIR>/..)
