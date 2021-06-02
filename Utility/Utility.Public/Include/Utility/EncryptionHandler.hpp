#pragma once
#include "Handler.hpp"

namespace Network
{
/** @class EncryptionHandler
 *  @brief handler class for messages encryption.
 */
class EncryptionHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for encryption of outcoming messages.
     * @param message - buffer that contains data that should be encrypted.
     * @param headerBuffer - buffer that will contain encrypted header.
     * @param bodyBuffer - buffer that will contain encrypted body.
     */
    void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body encryption
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header encryption

        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    /**
     * @brief Method for decryption of incoming message headers.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message header data.
     */
    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        // header decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    /**
     * @brief Method for decryption of incoming message bodies.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message body.
     */
    void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        // body decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
}  // namespace Network
