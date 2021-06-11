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

target_link_libraries(${TARGET} PUBLIC Network.Public
								PUBLIC Utility.Public
								PUBLIC Utility.Static
								PUBLIC Client.Core.Static
								PUBLIC DataAccess.Static
								PUBLIC CONAN_PKG::sqlcipher
								PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio CONAN_PKG::yas)

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
