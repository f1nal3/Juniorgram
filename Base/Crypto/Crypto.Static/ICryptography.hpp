#pragma once

#include "Network/YasSerializer.hpp"
#include "Utility/Utility.hpp"

namespace Base::Crypto
{
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
    /// @brief Method for encryption. Gets params from EncrytptionHandler::handleOutcomingMessage
    virtual Utility::GeneralCodes encrypt(const std::string& message, yas::shared_buffer& bodyBuffer) = 0;
    /// @brief Method for decryption. Gets params from EncrytptionHandler::handleIncomingMessage
    virtual Utility::GeneralCodes decrypt(yas::shared_buffer& buffer, const std::string& message) = 0;
    virtual ~ICryptography()                                                            = default;
};
}  // namespace Base::Crypto
