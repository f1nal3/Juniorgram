set(TARGET Client.Core.Test)
set(OUTPUT_NAME Client.Core.Test)
set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES
    ClientCoreTest.cpp)

if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    Client.Core 
    Precompiled.Catch2.Main)

target_compile_definitions(${TARGET} PRIVATE CATCH_CONFIG_FAST_COMPILE)
target_precompile_headers(${TARGET} REUSE_FROM Precompiled.Catch2.Test)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
