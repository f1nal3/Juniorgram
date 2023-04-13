set(TARGET Base.Crypto.Test)
set(OUTPUT_NAME Base.Crypto.Test)

set(SOURCE_FILES
        CryptoTests.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PUBLIC 
    Precompiled.Catch2.Main
    Base.Crypto.Static)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Base.Crypto.Static,SOURCE_DIR>/..)
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Network.Public,SOURCE_DIR>/Include)
target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Public,SOURCE_DIR>/Include)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})

install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${TEST_INSTALL_DIR})
