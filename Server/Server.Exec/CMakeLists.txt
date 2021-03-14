set(TARGET Server.Exec)
set(OUTPUT_NAME Juniorgram.Server.Exec)

set(SOURCE_FILES
    MainServer.cpp)

if(WIN32)
    set(EXTRA_LIBS ws2_32 wsock32)
endif()

add_executable(${TARGET} ${SOURCE_FILES} "ArgumentParser.h" "KeysValidator.h")

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS} CONAN_PKG::asio)

target_link_libraries(${TARGET} PUBLIC Network.Public)

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Release)
