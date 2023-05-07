#pragma once

#include <cryptopp/secblock.h>

#include <string>

namespace Base::KeyAgreement
{
using CryptoPP::SecByteBlock;

/** @class IKeyAgreement
* @brief Interface determines used key agreement protocol
* @detatils Class contains pair of temporary key based on which will be created shared(session) key
*/
class IKeyAgreement
{
public:
    /// @brief Method to calculate shared key and process it
    virtual SecByteBlock calculateSharedKey(const SecByteBlock& publicOthersideKey) = 0;

    /// @brief Method for (re)generating pair of temporary keys
    virtual void generateKeys() = 0;

    SecByteBlock getPublicKey() { return _publicKey; };

protected:
    SecByteBlock _privateKey, _publicKey;
};
};  // namespace Base::KeyAgreement
