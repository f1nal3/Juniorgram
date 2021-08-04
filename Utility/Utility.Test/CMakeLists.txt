set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES 
    ExceptionTest.cpp
    UtilityTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    CONAN_PKG::catch2
    Utility.Public
    Utility.Static)



add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
