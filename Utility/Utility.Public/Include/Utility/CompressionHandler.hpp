#pragma once

#include <lz4.h>

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
    suppressWarning(4018, Init)
    std::pair<std::unique_ptr<char[]>, size_t> compress(const void* data, size_t dataSize)
    {
        if (dataSize == 0 || data == nullptr)
        {
            throw Utility::CompressionException(
                "You cannot perform a compression process with a compressible data size equal to "
                "zero or NULL pointer!",
                __FILE__, __LINE__);
        }

        unsigned int csizeBound = LZ4_compressBound((int)dataSize);


        std::unique_ptr<char[]> compressedData(new char[csizeBound]);
        int compressedBytes = LZ4_compress_default(static_cast<const char*>(data),
                                                   compressedData.get(), (int)dataSize, csizeBound);

        if (compressedBytes < 0)
        {
            throw Utility::CompressionException(
                "Destination buffer is not large enough or source stream is detected malformed!",
                __FILE__, __LINE__);
        }

        if (!(compressedBytes >= 0 && compressedBytes <= csizeBound))
        {
            throw Utility::CompressionException(
                "Destination buffer is not large enough or source stream is detected malformed!",
                __FILE__, __LINE__);
        }


        return std::make_pair(std::move(compressedData), compressedBytes);
    }
    restoreWarning

    std::pair<std::unique_ptr<char[]>, size_t> decompress(const void* data,
                                                          unsigned int dataSize)
    {
        if (data == nullptr && dataSize <= 0)
        {
            throw Utility::CompressionException(
                "You cannot perform a decompression process with a decompressible data size equal to "
                "zero or NULL pointer!",
                __FILE__, __LINE__);
        }

        std::unique_ptr<char[]> decompressedData(new char[mDestinationCapacity]);

        int decompressedBytes = LZ4_decompress_safe(
            static_cast<const char*>(data), static_cast<char*>(decompressedData.get()),
                                                    (int)dataSize, mDestinationCapacity);

        if (decompressedBytes < 0)
        {
            throw Utility::CompressionException(
                "Destination buffer is not large enough or source stream is detected malformed!",
                __FILE__, __LINE__);
        }
        
        //This is specific tests for decompression process
        // assert(result >= 0);
        // assert(/*static_cast<size_t>(decompressedBytes)*/ == destSize);

        return std::make_pair(std::move(decompressedData), decompressedBytes);
    }

    MessageProcessingState handleOutcomingMessage(Message& message,
                                yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body compression
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header compression
    
        std::string strBodyBuffer = std::string(bodyBuffer.data.get(), bodyBuffer.size);
       
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
        std::string strBodyBuffer = std::string(buffer.data.get(), buffer.size);
       
        std::pair<std::unique_ptr<char[]>, size_t> decompressedBuffer =
            decompress(strBodyBuffer.data(), (unsigned int)strBodyBuffer.size());

        buffer.assign(decompressedBuffer.first.get(), decompressedBuffer.second);

        if (this->nextHandler)
        {
          return this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }

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
