set(TARGET Network.Public)
set(OUTPUT_NAME ${PROJECT}.Network.Public)

set(SOURCE_FILES
    Include/Network/CompressionHandler.hpp
    Include/Network/Connection.hpp
    Include/Network/EncryptionHandler.hpp
    Include/Network/Handler.hpp
    Include/Network/Message.hpp
    Include/Network/SerializationHandler.hpp
    Include/Network/YasSerializer.hpp
    Include/Network/iAPI.hpp)

add_library(${TARGET} INTERFACE ${SOURCE_FILES})

target_sources(${TARGET} INTERFACE ${SOURCE_FILES})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

target_link_libraries(${TARGET} INTERFACE yas::yas 
                                            asio::asio
                                            Utility.Public 
                                            Base.Logger.Static 
                                            Models.Public)
