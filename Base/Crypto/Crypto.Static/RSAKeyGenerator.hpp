#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include "FileLogger.hpp"
#include "Models/Primitives.hpp"

namespace Base
{
    struct RSAKeyPair
    {
        CryptoPP::RSA::PublicKey  publicKey;
        CryptoPP::RSA::PrivateKey  privateKey;

        std::string getPublicKeyAsString()
        {
            std::string publicKeyStr;
            CryptoPP::StringSink  stringSink(publicKeyStr);
            publicKey.DEREncode(stringSink);
            return publicKeyStr;
        }

        CryptoPP::RSA::PublicKey getPublicKeyFromString(const std::string& publicServerKeyStr)
        {
            CryptoPP::StringSource   stringSource(publicServerKeyStr, true);
            CryptoPP::RSA::PublicKey publicServerKey;
            publicServerKey.BERDecode(stringSource);
            return publicServerKey;
        }
    };
}

namespace Base::Generators
{
using Base::Logger::FileLogger;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::RSA;
using Base::RSAKeyPair;

/** @class RSAKeyGenerator
 * @brief Generator of RSA keys
 * @details Generates RSA key pair or public key based on private key
 */
class RSAKeyGenerator
{
public:
    /** @enum RSASecurityStrength
     * @brief Determines length of RSA key
     * @details Look here: https://www.keylength.com/en/4/
     */
    enum RSASecurityStrength : uint32_t
    {
        test = 1024,
        _128 = 3072,
        _192 = 7680,
        _256 = 15360,
    };

    /// @brief Method for generating key pair
    RSAKeyPair generateRSAKeyPair(RSASecurityStrength _secStrength = _128)
    {
        RSAKeyPair rsaKeyPair;

        FileLogger::getInstance().log("Start generating private and public keys. It will take some time", Base::Logger::LogLevel::INFO);
        AutoSeededRandomPool _randPool;
        rsaKeyPair.privateKey.GenerateRandomWithKeySize(_randPool, _secStrength);
        rsaKeyPair.publicKey = RSA::PublicKey(rsaKeyPair.privateKey);
        FileLogger::getInstance().log("Private and public key generation finished", Base::Logger::LogLevel::INFO);

        return rsaKeyPair;
    }
    /// @brief Method for generating public key based on private key
    RSA::PublicKey generateRSAPublicKey(const RSA::PrivateKey& privateKey) { return RSA::PublicKey(privateKey); };
};
}  // namespace Base::Generators
