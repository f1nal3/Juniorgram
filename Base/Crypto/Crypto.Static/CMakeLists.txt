set(TARGET Base.Crypto.Static)
set(OUTPUT_NAME Base.Crypto.Static)

set(SOURCE_FILES
    Hashing.hpp
    Hashing.cpp
    SessionKeyHolder.hpp
    ICryptography.hpp
    AES_GCM.hpp
    KeyPairManager.hpp
    RSAKeyGenerator.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC cryptopp::cryptopp)

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
