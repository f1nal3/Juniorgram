set(TARGET Client.CLI)
 
set(SOURCE_FILES
   MainClient.cpp
   App.cpp
   App.hpp)
 
add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:DataAccess.Static,SOURCE_DIR>/..)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} 
    PUBLIC Utility.Static                      
    PUBLIC Utility.Public
	PUBLIC DataAccess.Static
    PUBLIC Network.Public
	PUBLIC Client.Core.Static
	PUBLIC CONAN_PKG::sqlcipher
	PUBLIC CONAN_PKG::asio
	PUBLIC CONAN_PKG::yas
    PUBLIC CONAN_PKG::lz4
	PRIVATE ${EXTRA_LIBS})

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
