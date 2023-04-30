#pragma once

#include "Network/YasSerializer.hpp"

namespace Base::Crypto
{
    using Network::EncryptionState;
/** @class ICryptography
 * @brief Interface for encryption schemes
 * @details The intreface provides access to encryption and decryption for EncryptionHandler class.
 * Object contained in Connection class and provide changing encryption algorithm at runtime.
 * Due to https://github.com/f1nal3/Juniorgram/blob/dev/Docs/Cryptography.md the typical use is
 * selection encryption scheme on creating Connection.
 */
class ICryptography
{
public:
    /** @brief Method for encryption
    * @param buffer[in,out] Reference to serialized and compressed data in buffer. After encryption will be substituted on
    * encrypted data.
    * @param key[in] Encryption key
    * @param initVector[in] Initialization vector (may be unused in some implementation of encryption)
    * @param authData[in] Authentification data, used for data authentification (may be unused)
    */
    virtual EncryptionState encrypt(yas::shared_buffer& buffer,
                                    const SecByteBlock& key,
                                    const SecByteBlock& initVector = SecByteBlock(0),
                                    const std::string& authData = "") = 0;

    /** @brief Method for decryption
    * @param buffer[in,out] Reference to serialized, compressed and encrypted data in buffer
    * @param key[in] Encryption key
    * @param initVector[in] Initialization vector (may be unused in some implementation of encryption)
    * @param authData[in] Authentification data, used for data authentification (may be unused)
    */
    virtual EncryptionState decrypt(yas::shared_buffer& buffer,
                                    const SecByteBlock& key,
                                    const SecByteBlock& initVector = SecByteBlock(0),
                                    const std::string& authData = "") = 0;

    virtual ~ICryptography() = default;
};
}  // namespace Base::Crypto
