set(TARGET Network.Test)
set(OUTPUT_NAME Network.Test)

set(SOURCE_FILES
    NetworkTest.cpp
    TimestampTest.cpp)

if(WIN32)
    set(EXTRA_LIBS ws2_32 wsock32)
endif()

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} Network.Static CONAN_PKG::asio CONAN_PKG::cryptopp CONAN_PKG::lz4)

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public CONAN_PKG::catch2)

add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
