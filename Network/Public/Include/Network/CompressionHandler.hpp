#pragma once
#include "Network/Handler.hpp"

namespace Network
{
/** 
* @class CompressionHandler
* @brief handler class for messages compression.
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
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
    * @brief Method for decompression of incoming message bodies.
    * @param buffer - buffer that contains data that should be decompressed.
    * @param messageHeader - variable that will contain decompressed message body.
    */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        /// body decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  /// namespace Network
