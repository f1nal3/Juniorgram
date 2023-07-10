set(TARGET Base.Crypto.Static)
set(OUTPUT_NAME Base.Crypto.Static)

set(SOURCE_FILES
    AES_GCM.hpp
    CryptoPrimitives.hpp
    ECDH.hpp
    Hashing.hpp
    Hashing.cpp
    HashVerifier.hpp
    IConnectionVerifier.hpp
    ICryptography.hpp
    IKeyAgreement.hpp
    KeyConfirmation.hpp
    RSA.hpp
    RSAKeyGenerator.hpp
    RSAKeyManager.hpp
    SessionKeyHolder.hpp)

add_library(${TARGET} STATIC ${SOURCE_FILES})

target_link_libraries(${TARGET} PUBLIC cryptopp::cryptopp
    Network.Public
    Models.Public
    Utility.Public)

set_target_properties(${TARGET} PROPERTIES
    LINKER_LANGUAGE CXX
    FOLDER ${FOLDER})

target_include_directories(${TARGET} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
