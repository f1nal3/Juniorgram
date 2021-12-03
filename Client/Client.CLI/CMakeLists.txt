set(TARGET Client.CLI)
 
set(SOURCE_FILES
   MainClient.cpp
   App.cpp
   App.hpp)
 
add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public)
target_link_libraries(${TARGET} PUBLIC Client.Core.Static)
target_link_libraries(${TARGET} PUBLIC Base.Logger.Static)
target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio CONAN_PKG::yas)

target_include_directories(${TARGET} PRIVATE
    $<TARGET_PROPERTY:Base.Logger.Static,SOURCE_DIR>/..)

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Client/Release)
