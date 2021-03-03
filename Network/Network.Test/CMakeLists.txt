set(TARGET Network.Test)
set(OUTPUT_NAME Network.Test)

set(SOURCE_FILES
    NetworkTest.cpp)

set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC Network.Public CONAN_PKG::catch2)

enable_testing()
add_test(NAME ${OUTPUT_NAME} COMMAND ${TARGET})
