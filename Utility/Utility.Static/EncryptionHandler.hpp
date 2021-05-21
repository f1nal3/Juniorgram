#pragma once
#include "Handler.hpp"

namespace Network
{
class EncryptionHandler : public AbstractHandler
{
public:
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

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        // header decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

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
