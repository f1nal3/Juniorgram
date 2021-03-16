set(TARGET Client.CLI)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(SOURCE_FILES
   MainClient.cpp
    )
 
add_executable(${TARGET} ${SOURCE_FILES})
 
target_link_libraries(${TARGET} PUBLIC Network.Public)
target_link_libraries(${TARGET} PUBLIC Client.Core.Static)
 
install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
