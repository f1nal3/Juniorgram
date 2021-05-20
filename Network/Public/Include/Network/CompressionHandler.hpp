#pragma once
#include "Handler.hpp"

class CompressionHandler : public AbstractHandler
{
public:
    void handleOutcomingMessage(const Network::Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Network::Message::MessageHeader messageHeader = message.mHeader;
        // body compression
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header compression

        if (!this->nextHandler)
        {
            AbstractHandler::handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Network::Message::MessageHeader& messageHeader) override
    {
        // header decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    void handleIncomingMessageBody(const yas::shared_buffer buffer,
                                   Network::Message& message) override
    {
        // body decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
