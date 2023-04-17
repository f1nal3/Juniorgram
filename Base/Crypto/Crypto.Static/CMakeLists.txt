set(TARGET Base.Crypto.Static)
set(OUTPUT_NAME Base.Crypto.Static)

set(SOURCE_FILES
    Hashing.hpp
    Hashing.cpp
    SessionKeyHolder.hpp
    ICryptography.hpp
    AES_GCM.hpp
    RSAKeyManager.hpp
    RSAKeyGenerator.hpp
    RSA.hpp
    IConnectionVerifier.hpp
    HashVerifier.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC cryptopp::cryptopp
    Network.Public
    Models.Public
    Utility.Public)

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
