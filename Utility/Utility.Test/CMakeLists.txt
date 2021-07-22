set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES 
    BinarySerializeTest.cpp
    ExceptionTest.cpp
    JsonSerializeTest.cpp
    UtilityTest.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} PRIVATE Network.Static Utility.Static CONAN_PKG::catch2 CONAN_PKG::lz4)

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
