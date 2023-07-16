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
    * @param source - variable that contains data that should be compressed.
    * @param destination - buffer that will contain compressed message body.
    */
    MessageProcessingState handleOutcomingMessage(const Message& source, yas::shared_buffer& destination) override
    {
        if (this->nextHandler)
        {
            this->nextHandler->handleOutcomingMessage(source, destination);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
    * @brief Method for decompression of incoming message bodies.
    * @param source - buffer that contains data that should be decompressed.
    * @param destination - variable that will contain decompressed message body.
    */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer& source, Message& destination) override
    {
        /// body decompression

        if (this->nextHandler)
        {
            this->nextHandler->handleIncomingMessageBody(source, destination);
        }
        return MessageProcessingState::SUCCESS;
    }
};
}  /// namespace Network
