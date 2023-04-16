#pragma once

#include "Network/YasSerializer.hpp"
#include "Utility/Utility.hpp"
#include "Network/Message.hpp"

namespace Base::Crypto
{
using Network::Message;

/** @class ICryptography
 * @brief Interface for encryption schemes
 * @detatils The intreface provides access to encryption and decryption for EncryptionHandler class.
 * Object contained in Connection class and provide changing encryption algorithm at runtime.
 * Due to https://github.com/f1nal3/Juniorgram/blob/dev/Docs/Cryptography.md the typical use is
 * selection encryption scheme on creating Connection.
 */
class ICryptography
{
public:
    /// @brief Method for encryption. Repeats params of EncrytptionHandler::handleOutcomingMessage
    virtual Utility::GeneralCodes encrypt(const Message& message, yas::shared_buffer& bodyBuffer) = 0;
    /// @brief Method for decryption. Repeats params of EncrytptionHandler::handleIncomingMessage
    virtual Utility::GeneralCodes decrypt(yas::shared_buffer& buffer, Message& message) = 0;
    virtual ~ICryptography()                                                            = default;
};
}  // namespace Base::Crypto
