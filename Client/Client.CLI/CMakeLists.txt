set(TARGET Client.CLI)
 
set(SOURCE_FILES
   MainClient.cpp
   App.cpp
   App.hpp)
 
add_executable(${TARGET} ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public
								PUBLIC Client.Core.Static
								PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio CONAN_PKG::yas)

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
