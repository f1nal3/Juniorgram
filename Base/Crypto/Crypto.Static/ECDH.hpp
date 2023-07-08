#pragma once

#include <cryptopp/osrng.h>
#include <cryptopp/secblock.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/asn.h>

#include "SessionKeyHolder.hpp"
#include "IKeyAgreement.hpp"
#include "FileLogger.hpp"

namespace Base::KeyAgreement
{
    using Base::Logger::FileLogger;
    using Base::SessionKeyHolder;
    using Base::Hashing::SHA_256;
    using CryptoPP::ASN1::secp256r1;
    using CryptoPP::AutoSeededRandomPool;
    using CryptoPP::SecByteBlock;
    using CryptoPP::OID;
    using CryptoPP::byte;
    using CryptoPP::ECP;
    using Domain =  CryptoPP::ECDH<ECP>::Domain;

/** @class ECDH
* @brief Class implements Elliptic Curve Diffie-Hellman key agreement
* @details The class generates temporary keys for subsequent generation of shared key (secret).
* Before placing the shared key in the session key storage, class can process it.
*/
class ECDH : public IKeyAgreement
{
public:
    /// @brief Method for (re)generating temporary keys
    void generateKeys() { createEphemeralKeys(); };

    /** @brief Server's method for calculating shared key
    * @details Method places shared key in SessionKeyHolder for client with clientId.
    */
    bool calculateSharedKey(const std::string& publicClientKeyStr, uint64_t clientId = 0)
    {
        SecByteBlock sharedKey(_domain.AgreedValueLength());
        SecByteBlock publicOthersideKey(reinterpret_cast<const byte*>(publicClientKeyStr.data()), publicClientKeyStr.size());

        bool correctness = _domain.Agree(sharedKey, _privateKey, publicOthersideKey);
        if (correctness)
        {
            SessionKeyHolder::Instance().addUserKey(cutInHalf(sharedKey), clientId);
            FileLogger::getInstance().log("Session key for userId = " + std::to_string(clientId) + "is setted",
                                          Base::Logger::LogLevel::INFO);
        }
        else
        {
            FileLogger::getInstance().log("Failed to reach shared secret", Base::Logger::LogLevel::ERR);
        }
        clearKeys();
        return correctness;
    }

    /** @brief Client's method for calculating shared key
    * @details Method places shared key in SessionKeyHolder.
    */
    bool calculateSharedKey(const std::string& publicOthersideKeyStr)
    {
        SecByteBlock sharedKey(_domain.AgreedValueLength());
        SecByteBlock publicOthersideKey(reinterpret_cast<const byte*>(publicOthersideKeyStr.data()), publicOthersideKeyStr.size());

        bool correctness = _domain.Agree(sharedKey, _privateKey, publicOthersideKey);
        if (correctness)
        {
            SessionKeyHolder::Instance().setKey(cutInHalf(sharedKey));
            FileLogger::getInstance().log("Session key is setted", Base::Logger::LogLevel::INFO);
        }
        else
        {
            FileLogger::getInstance().log("Failed to reach shared secret", Base::Logger::LogLevel::ERR);
        }
        clearKeys();
        return correctness;
    }

private:
    AutoSeededRandomPool _randPool;
    SecByteBlock         _privateKey, _publicKey;
    /// Defines a curve for calculating the public key
    OID                  _curve  = secp256r1();
    Domain               _domain = Domain(_curve);

    /// @brief Method for (re)generating private and public keys for further creating shared key
    inline void createEphemeralKeys()
    {
        _privateKey.New(_domain.PrivateKeyLength());
        _publicKey.New(_domain.PublicKeyLength());

        _domain.GenerateKeyPair(_randPool, _privateKey, _publicKey);
    }

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
