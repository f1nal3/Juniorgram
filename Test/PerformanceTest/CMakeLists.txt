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
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/PerformanceTest/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/PerformanceTest/Release)

