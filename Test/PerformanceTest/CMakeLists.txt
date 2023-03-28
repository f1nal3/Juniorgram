project(PerformanceTest LANGUAGES CXX)

set(TARGET PerformanceTest)
set(SOURCE_FILES
    ArgumentParser.hpp
    ArgumentParser.cpp
	main.cpp
	SimpleTimer.hpp
    SimpleTimer.cpp
    Tester.hpp
    Tester.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${TEST_INSTALL_DIR})

