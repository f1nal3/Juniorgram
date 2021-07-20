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

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} 
    PRIVATE ${EXTRA_LIBS} Utility.Static Network.Static DataAccess.Static Client.Core.Static CONAN_PKG::asio CONAN_PKG::nlohmann_json	
    CONAN_PKG::lz4
	CONAN_PKG::sqlcipher
	CONAN_PKG::yas
    CONAN_PKG::cryptopp)

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public)
    

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
