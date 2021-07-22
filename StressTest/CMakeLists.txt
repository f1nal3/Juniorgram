project(StressTest LANGUAGES CXX)

set(TARGET StressTest)
set(SOURCE_FILES
	main.cpp
	SimpleTimer.hpp
    SimpleTimer.cpp
    ArgumentParser.hpp
    ArgumentParser.cpp)

add_executable(${TARGET} ${SOURCE_FILES})

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/StressTest/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/StressTest/Release)

