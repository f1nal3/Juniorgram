set(TARGET Client.CLI)
 
set(SOURCE_FILES
   MainClient.cpp
   App.cpp
   App.hpp)
 
add_executable(${TARGET} ${SOURCE_FILES})

set_target_properties(${TARGET} PROPERTIES FOLDER ${FOLDER})

target_link_libraries(${TARGET} PUBLIC Network.Public 
                                        Utility.Public
                                        Client.Core.Static
                                PRIVATE ${EXTRA_LIBS} asio::asio yas::yas)

install(TARGETS ${TARGET}
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
