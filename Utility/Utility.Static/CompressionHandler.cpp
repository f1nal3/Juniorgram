#include <Network.Static/Message.hpp>
#include "CompressionHandler.hpp"

namespace Utility
{
std::pair<std::unique_ptr<char[]>, size_t> CompressionHandler::compress(const void* data,
                                                                        size_t dataSize)
{
    if (dataSize == 0 || data == nullptr)
    {
        throw Utility::CompressionException(
            "You cannot perform a compression process with a compressible data size equal to "
            "zero or NULL pointer!",
            __FILE__, __LINE__);
    }

    int csizeBound = LZ4_compressBound((int)dataSize);

    std::unique_ptr<char[]> compressedData(new char[csizeBound]);
    int compressedBytes = LZ4_compress_default(static_cast<const char*>(data), compressedData.get(),
                                               (int)dataSize, csizeBound);

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

std::pair<std::unique_ptr<char[]>, size_t> CompressionHandler::decompress(const void* data, const unsigned int& dataSize, const unsigned int& originalSize)
{
    if (data == nullptr && dataSize <= 0)
    {
        throw Utility::CompressionException(
            "You cannot perform a decompression process with a decompressible data size equal "
            "to "
            "zero or NULL pointer!",
            __FILE__, __LINE__);
    }

    std::unique_ptr<char[]> decompressedData(new char[originalSize]);

    int decompressedBytes = LZ4_decompress_safe(static_cast<const char*>(data),
                                                static_cast<char*>(decompressedData.get()),
                                                (int)dataSize, originalSize);

    if (decompressedBytes < 0)
    {
        throw Utility::CompressionException(
            "Destination buffer is not large enough or source stream is detected malformed!",
            __FILE__, __LINE__);
    }

    // This is specific tests for decompression process
    // assert(result >= 0);
    // assert(/*static_cast<size_t>(decompressedBytes)*/ == destSize);

    return std::make_pair(std::move(decompressedData), decompressedBytes);
}

Utility::MessageProcessingState CompressionHandler::handleOutcomingMessage(
    Network::Message& message, yas::shared_buffer& bodyBuffer)
{
    // Message::MessageHeader messageHeader = message.mHeader;
    // body compression
    // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
    // header compression

    std::string strBodyBuffer = std::string(bodyBuffer.data.get(), bodyBuffer.size);

    suppressWarning(4267, Init) 
    message.mHeader.mOriginalSize = bodyBuffer.size;
    restoreWarning

    std::pair<std::unique_ptr<char[]>, size_t> compressedBuffer =
        compress(strBodyBuffer.data(), strBodyBuffer.size());

    bodyBuffer.assign(compressedBuffer.first.get(), compressedBuffer.second);

    if (this->nextHandler)
    {
        return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
    }
    return Utility::MessageProcessingState::SUCCESS;
}

Utility::MessageProcessingState CompressionHandler::handleIncomingMessageBody(
    yas::shared_buffer buffer, Network::Message& message)
{
    std::string strBodyBuffer = std::string(buffer.data.get(), buffer.size);

    std::pair<std::unique_ptr<char[]>, size_t> decompressedBuffer =
        decompress(strBodyBuffer.data(), (unsigned int)strBodyBuffer.size(), message.mHeader.mOriginalSize);

    buffer.assign(decompressedBuffer.first.get(), decompressedBuffer.second);

    if (this->nextHandler)
    {
        return this->nextHandler->handleIncomingMessageBody(buffer, message);
    }
    return Utility::MessageProcessingState::SUCCESS;
}
}  // namespace Network