#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/asn.h>

#include "IKeyAgreement.hpp"
#include "FileLogger.hpp"

namespace Base::KeyAgreement
{
using Base::Logger::FileLogger;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::byte;
using CryptoPP::ECP;
using CryptoPP::OID;
using CryptoPP::ASN1::secp256r1;
using Domain = CryptoPP::ECDH<ECP>::Domain;

/** @class ECDH
 * @brief Class implements Elliptic Curve Diffie-Hellman key agreement
 * @details The class generates temporary keys for subsequent generation of shared key (secret).
 * Before placing the shared key in the session key storage, class can process it.
 */
class ECDH final : public IKeyAgreement
{
public:
    /// @brief Method for (re)generating temporary(ephemeral) keys
    void generateKeys()
    {
        _privateKey.New(_domain.PrivateKeyLength());
        _publicKey.New(_domain.PublicKeyLength());

        _domain.GenerateKeyPair(_randPool, _privateKey, _publicKey);
    };

    /* @brief Method for calculating shared key.
    * @return Success: some shared key with 128 bits size \
    * Failure: empty SecByteBlock object
    */
    SecByteBlock calculateSharedKey(const SecByteBlock& publicOthersideKey)
    {
        SecByteBlock sharedKey(_domain.AgreedValueLength());

        bool correctness = _domain.Agree(sharedKey, _privateKey, publicOthersideKey);

        if (correctness)
        {
            return cutInHalf(sharedKey);
        }
        else
        {
            FileLogger::getInstance().log("Failed to reach ECDH shared secret", Base::Logger::LogLevel::ERR);
        }
        return SecByteBlock(0);
    }

private:
    AutoSeededRandomPool _randPool;
    OID                  _curve  = secp256r1();  /// defines a curve for calculating the public key
    Domain               _domain = Domain(_curve);

    /// @brief Method clears keys after generation shared key (it does not matter, successful or unsuccessful)
    inline void clearKeys()
    {
        _privateKey.CleanNew(0);
        _publicKey.CleanNew(0);
    }

    /// @brief Optional method for preparation shared key berofe placing it to session key storage
    inline SecByteBlock cutInHalf(const SecByteBlock& keyToCut, bool useXor = true)
    {
        if (useXor)
        {
            size_t       halfSize = keyToCut.size() / 2;
            SecByteBlock cuttedKey(halfSize);

            CryptoPP::xorbuf(cuttedKey, keyToCut.begin(), keyToCut.begin() + halfSize, halfSize);
            return cuttedKey;
        }
        return SecByteBlock(keyToCut.begin(), keyToCut.size() / 2);
    }
};
}  // namespace Base::KeyAgreement
