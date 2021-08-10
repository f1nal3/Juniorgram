#pragma once

#include <lz4.h>
#include "Utility/Exception.hpp"
#include "Handler.hpp"


namespace Utility
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
    suppressWarning(4018, Init) 
    std::pair<std::unique_ptr<char[]>, size_t> compress(const void* data, size_t dataSize);
    restoreWarning

    std::pair<std::unique_ptr<char[]>, size_t> decompress(const void* data, const unsigned int& dataSize, const unsigned int& originalSize);

    MessageProcessingState handleOutcomingMessage(Network::Message& message,
                                                           yas::shared_buffer& bodyBuffer) override;

    /**
     * @brief Method for de
     ion of incoming message bodies.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                     Network::Message& message) override;
};
}  // namespace Network
