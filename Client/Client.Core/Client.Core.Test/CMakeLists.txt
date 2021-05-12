set(TARGET Client.Core.Test)
set(OUTPUT_NAME Client.Core.Test)

set(SOURCE_FILES
    ClientCoreTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    Client.Core 
    CONAN_PKG::catch2)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
