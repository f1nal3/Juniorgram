set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES 
    BinarySerializeTest.cpp
    ExceptionTest.cpp
    JsonSerializeTest.cpp
    UtilityTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    CONAN_PKG::catch2
    Utility.Public
    Utility.Static)

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
