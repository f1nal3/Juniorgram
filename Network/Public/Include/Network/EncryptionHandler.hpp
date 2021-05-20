#pragma once
#include "Handler.hpp"

class EncryptionHandler : public AbstractHandler
{
public:
    void handleOutcomingMessage(const Network::Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Network::Message::MessageHeader messageHeader = message.mHeader;
        // body encryption
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header encryption

        if (!this->nextHandler)
        {
            AbstractHandler::handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Network::Message::MessageHeader& messageHeader) override
    {
        // header decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    void handleIncomingMessageBody(const yas::shared_buffer buffer,
                                   Network::Message& message) override
    {
        // body decryption

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
