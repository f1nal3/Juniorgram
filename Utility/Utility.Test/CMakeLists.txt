set(TARGET Utility.Test)
set(OUTPUT_NAME Utility.Test)

set(SOURCE_FILES
    UtilityTest.cpp
    SerializeTest.cpp
    ExceptionTest.cpp)

#set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC Utility.Public Utility.Static CONAN_PKG::catch2)
target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..
    )

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
