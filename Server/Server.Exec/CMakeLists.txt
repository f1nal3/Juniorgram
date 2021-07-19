set(TARGET Server.Exec)
set(OUTPUT_NAME Juniorgram.Server.Exec)

set(SOURCE_FILES
    MainServer.cpp)

if(WIN32)
    set(EXTRA_LIBS ws2_32 wsock32)
endif()

add_executable(${TARGET} ${SOURCE_FILES})

target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Network.Static,SOURCE_DIR>/..)
    
target_include_directories(${TARGET}
    PRIVATE ${CONAN_INCLUDE_DIRS_QT}
    $<TARGET_PROPERTY:Utility.Static,SOURCE_DIR>/..)

target_link_libraries(${TARGET} PRIVATE ${EXTRA_LIBS}  Server.Core.Static Network.Static Utility.Static CONAN_PKG::asio CONAN_PKG::yas CONAN_PKG::lz4 CONAN_PKG::nlohmann_json)
target_link_libraries(${TARGET} PUBLIC Network.Public Utility.Public)

install(TARGETS ${TARGET}
        CONFIGURATIONS Debug
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Debug)
install(TARGETS ${TARGET}
        CONFIGURATIONS Release
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin/Server/Release)
