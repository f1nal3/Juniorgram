#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include "Utility/Utility.hpp"
#include "Utility/JGExceptions.hpp"

namespace Base::Generators
{
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::RSA;
using Models::RSAKeyPair;

/* @class RSAKeyGenerator
 * @brief Generator of RSA keys
 * @details Generates RSA key pair or public key based on private key
 */
class RSAKeyGenerator
{
public:
    /// @brief Method for generating key pair
    RSAKeyPair generateRSAKeyPair()
    {
        RSAKeyPair           rsaKeyPair;
        AutoSeededRandomPool randPool;

        rsaKeyPair.privateKey.GenerateRandomWithKeySize(randPool, 3072);
        rsaKeyPair.publicKey = RSA::PublicKey(rsaKeyPair.privateKey);
        return rsaKeyPair;
    }
    /// @brief Method for generating public key based on private key
    RSA::PublicKey generateRSAPublicKey(const RSA::PrivateKey& privateKey) { return RSA::PublicKey(privateKey); };
};
};  // namespace Base
