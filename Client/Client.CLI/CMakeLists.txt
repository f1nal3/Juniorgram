set(TARGET Client.CLI)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
 
set(SOURCE_FILES
   MainClient.cpp
   App.cpp)
	
 set(HEADER_FILES
   App.hpp)
 
add_executable(${TARGET} ${SOURCE_FILES} ${HEADER_FILES})
 
target_link_libraries(${TARGET} PUBLIC Network.Public)
target_link_libraries(${TARGET} PUBLIC Client.Core.Static)
target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio)
 
install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
