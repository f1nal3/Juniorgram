#pragma once
#include "Handler.hpp"
#include "SessionKeyHolder.hpp"
#include "ICryptography.hpp"

namespace Network
{
/** @class EncryptionHandler
 *  @brief handler class for messages encryption.
 */
class EncryptionHandler : public AbstractHandler
{
    std::shared_ptr<Base::Crypto::ICryptography> _cryptoAlgorithm;

public:
    EncryptionHandler(std::shared_ptr<Base::Crypto::ICryptography> cryptoAlgorithm_) :
        _cryptoAlgorithm(cryptoAlgorithm_)
    {
    };
    /**
     * @brief Method for encryption of outcoming messages.
     * @param message - buffer that contains data that should be encrypted.
     * @param headerBuffer - buffer that will contain encrypted header.
     * @param bodyBuffer - buffer that will contain encrypted body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
     * @brief Method for decryption of incoming message bodies.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  // namespace Network
