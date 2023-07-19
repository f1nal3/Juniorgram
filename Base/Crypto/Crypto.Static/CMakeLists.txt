set(TARGET Base.Crypto.Static)
set(OUTPUT_NAME Base.Crypto.Static)

set(SOURCE_FILES
    Encryption/AES_GCM.hpp
    CryptoPrimitives.hpp
    KeyAgreement/ECDH.hpp
    Hashing.hpp
    Hashing.cpp
    ConnectionVerifiers/HashVerifier.hpp
    ConnectionVerifiers/IConnectionVerifier.hpp
    Encryption/ICryptography.hpp
    KeyAgreement/IKeyAgreement.hpp
    KeyConfirmation.hpp
    RSA/RSA.hpp
    RSA/RSAKeyGenerator.hpp
    RSA/RSAKeyManager.hpp
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
