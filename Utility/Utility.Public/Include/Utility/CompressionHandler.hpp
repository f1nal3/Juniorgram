#pragma once

#include "lz4.h"

#include "Handler.hpp"

#include <sstream>

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

    std::pair<std::unique_ptr<char[]>, size_t> compress(const void* data, size_t dataSize)
    {
        if (dataSize == 0) return std::make_pair(std::unique_ptr<char[]>(), 0);

        unsigned int csizeBound = LZ4_compressBound((int)dataSize);

        std::unique_ptr<char[]> compressedData(new char[csizeBound]);

        int compressedBytes = LZ4_compress_default(static_cast<const char*>(data),
                                                   compressedData.get(),
                                                     (int)dataSize, csizeBound);

        if (compressedBytes < 0)
        {
            std::cout << "res < 0\n";
        }
        // assert(csize >= 0 && csize <= csizeBound);

        return std::make_pair(std::move(compressedData), compressedBytes);
    }

    std::pair<std::unique_ptr<char[]>, size_t> decompress(const void* source,
                                                          unsigned int sourceSize)
    {
        /*if (sourceSize == 0 && destSize == 0) return;*/

        std::unique_ptr<char[]> decompressedData(new char[256]);

        int decompressedBytes =
            LZ4_decompress_safe(static_cast<const char*>(source),
                                static_cast<char*>(decompressedData.get()),
                                (int)sourceSize, 256);

        // assert(result >= 0);
        // assert(/*static_cast<size_t>(*/result/*)*/ == destSize);

        return std::make_pair(std::move(decompressedData), decompressedBytes);
    }

    MessageProcessingState handleOutcomingMessage(Message& message,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body compression
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header compression
    
        std::string strBodyBuffer = std::string((char*)bodyBuffer.data.get(), bodyBuffer.size);
       
        std::pair<std::unique_ptr<char[]>, size_t> compressedBuffer =
            compress(strBodyBuffer.data(), strBodyBuffer.size());

        bodyBuffer.assign(compressedBuffer.first.get(), compressedBuffer.second);

        if (this->nextHandler)
        {
           return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
     * @brief Method for decompression of incoming message bodies.
     * @param buffer - buffer that contains data that should be decompressed.
     * @param messageHeader - variable that will contain decompressed message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                Message& message) override
    {
        std::string strBodyBuffer = std::string((char*)buffer.data.get(), buffer.size);
       
        std::pair<std::unique_ptr<char[]>, size_t> decompressedBuffer =
            decompress(strBodyBuffer.data(), (unsigned int)strBodyBuffer.size());

        buffer.assign(decompressedBuffer.first.get(), decompressedBuffer.second);

        if (this->nextHandler)
        {
          return this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
        
    }
};
}  // namespace Network
