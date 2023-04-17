#pragma once

#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/filters.h>

namespace Base::Crypto::Asymmetric
{
    using CryptoPP::AutoSeededRandomPool;
    using CryptoPP::RSAES;
    using CryptoPP::OAEP;
    using CryptoPP::SHA256;
    using CryptoPP::PK_EncryptorFilter;
    using CryptoPP::PK_DecryptorFilter;
    using CryptoPP::StringSource;
    using CryptoPP::StringSink;

/** @class RSA
* @brief Encryption class according to the RSA algorithm
* @details Class used by client to encrypt verification hash and by server to decrypt it.
* Due to using encryption by client and decryption by server only, no needs to create RSAKeyHolder.
*/
class RSA
{
public:
    /// @brief Encrytion function
    std::string encrypt(const std::string& hashForEncrypt, const std::string& publicServerKeyStr)
    {
        AutoSeededRandomPool randPool;
        std::string enctyptedHash;

        CryptoPP::RSA::PublicKey publicServerKey;
        StringSource             stringSource(publicServerKeyStr, true);
        publicServerKey.BERDecode(stringSource);

        RSAES<OAEP<SHA256>>::Encryptor encryptor(publicServerKey);

        StringSource strSource(hashForEncrypt, true,
            new PK_EncryptorFilter(randPool, encryptor,
                new StringSink(enctyptedHash)));

        return enctyptedHash;
    }

    std::string decrypt(const std::string& hashForDecrypt, CryptoPP::RSA::PrivateKey publicServerKey)
    {
        AutoSeededRandomPool randPool;
        std::string          decryptedHash;

        RSAES<OAEP<SHA256>>::Decryptor decryptor(publicServerKey);

        StringSource strSource(hashForDecrypt, true,
            new PK_DecryptorFilter(randPool, decryptor,
                new StringSink(decryptedHash)));

        return decryptedHash;
    }
};
};  // namespace Base::Crypto::Asymmetric