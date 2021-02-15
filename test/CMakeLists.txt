cmake_minimum_required(VERSION 3.10.0)

project(unit_tests)

file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)
set(CMAKE_BUILD_TYPE RELWITHDEBINFO)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

include_directories(${CONAN_INCLUDE_DIRS})

if (WIN32)
    list(APPEND EXTRA_LIBS ws2_32 wsock32)
endif ()
add_executable(unit_tests ${UNIT_TESTS_SRC_FILES} ${HEADERS})

target_link_libraries(unit_tests ${EXTRA_LIBS})
target_include_directories(unit_tests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../common")

enable_testing()
add_test(UnitTest unit_tests)
