#pragma once

#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>
#include <cryptopp/filters.h>

#include "FileLogger.hpp"

namespace Base::Crypto::Asymmetric
{
    using Base::Logger::FileLogger;
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
    /// @brief Encryption function
    std::string encrypt(const std::string& dataForEncrypt, const CryptoPP::RSA::PublicKey& publicServerKey)
    {
        std::string                    enctyptedData;
        RSAES<OAEP<SHA256>>::Encryptor encryptor(publicServerKey);

        if (!encryptor.FixedMaxPlaintextLength())
        {
            FileLogger::getInstance().log("[RSA] Invalid public key", Base::Logger::LogLevel::ERR);
            return "";
        }

        StringSource strSource(dataForEncrypt, true,
            new PK_EncryptorFilter(_randomGenerator, encryptor, new StringSink(enctyptedData)));

        return enctyptedData;
    }

    /// @brief Decryption function
    std::string decrypt(const std::string& dataForDecrypt, const CryptoPP::RSA::PrivateKey& privateServerKey)
    {
        std::string                    decryptedData;
        RSAES<OAEP<SHA256>>::Decryptor decryptor(privateServerKey);

        StringSource strSource(dataForDecrypt, true,
            new PK_DecryptorFilter(_randomGenerator, decryptor, new StringSink(decryptedData)));

        return decryptedData;
    }

private:
    AutoSeededRandomPool _randomGenerator;
};
}  // namespace Base::Crypto::Asymmetric
