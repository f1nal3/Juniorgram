#pragma once
#include "Handler.hpp"

namespace Network
{
/** @class CompressionHandler
 *  @brief handler class for messages compression.
 */
class CompressionHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for compression of outcoming messages.
     * @param message - buffer that contains data that should be compressed.
     * @param headerBuffer - buffer that will contain compressed header.
     * @param bodyBuffer - buffer that will contain compressed body.
     */
    void handleOutcomingMessage(const Message& message, yas::shared_buffer& headerBuffer,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body compression
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header compression

        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, headerBuffer, bodyBuffer);
        }
    }

    /**
     * @brief Method for decompression of incoming message headers.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message header data.
     */
    void handleIncomingMessageHeader(const yas::shared_buffer buffer,
                                     Message::MessageHeader& messageHeader) override
    {
        // header decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageHeader(buffer, messageHeader);
        }
    }

    /**
     * @brief Method for decompression of incoming message bodies.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message body.
     */
    void handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        // body decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
    }
};
}  // namespace Network
