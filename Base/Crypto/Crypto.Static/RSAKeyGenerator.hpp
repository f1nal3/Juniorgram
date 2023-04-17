#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/rsa.h>

#include "Models/Primitives.hpp"

namespace Base::Generators
{
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::RSA;
using Models::RSAKeyPair;

/** @class RSAKeyGenerator
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

        rsaKeyPair.privateKey.GenerateRandomWithKeySize(_randPool, _128);
        rsaKeyPair.publicKey = RSA::PublicKey(rsaKeyPair.privateKey);
        return rsaKeyPair;
    }
    /// @brief Method for generating public key based on private key
    RSA::PublicKey generateRSAPublicKey(const RSA::PrivateKey& privateKey) { return RSA::PublicKey(privateKey); };

private:
    AutoSeededRandomPool _randPool;

    /** @enum RSASecurityStrength
     * @brief Determines length of RSA key
     * @details Look here: https://www.keylength.com/en/4/
     */
    enum RSASecurityStrength : uint32_t
    {
        _128 = 3072,
        _192 = 7680,
        _256 = 15360,
    };
};
}  // namespace Base::Generators
