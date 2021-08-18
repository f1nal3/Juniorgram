set(TARGET Base.Test)
set(OUTPUT_NAME Base.Test)

set(SOURCE_FILES
    FileLoggerTests.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC 
    Precompiled.Catch2.Main
    Base.Public
    Base.Static)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Base.Static,SOURCE_DIR>/..)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
