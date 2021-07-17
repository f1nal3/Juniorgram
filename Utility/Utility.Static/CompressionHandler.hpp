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

    std::pair<std::unique_ptr<char[]>, size_t> decompress(const void* data,
                                                          unsigned int dataSize);

    MessageProcessingState handleOutcomingMessage(/*Network::Message& message,*/
                                                           yas::shared_buffer& bodyBuffer) override;

    /**
     * @brief Method for decompression of incoming message bodies.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer/*,
                                                              Network::Message& message*/) override;

private:
    /** 
    *DestinationCapacity - the function LZ4_decompress_safe (see
    * https://github.com/Cyan4973/lz4/blob/master/lib/lz4.c#L1288) seems to require only the
    * maximal decompressed size. In this case, you can allocate a buffer large enough for
    * decompression and use it. So either that, or transfer the original uncompressed size to the
    * remote machines as well.
    */
    const int mDestinationCapacity = 256;
};
}  // namespace Network
