#include "EncryptionHandler.hpp"

namespace Utility
{
Utility::MessageProcessingState EncryptionHandler::handleOutcomingMessage(
    /*Network::Message& message,*/ yas::shared_buffer& bodyBuffer)
{
    // Message::MessageHeader messageHeader = message.mHeader;
    // body encryption
    // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
    // header encryption
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(mEncryptionKey,
                                                                CryptoPP::AES::BLOCKSIZE, mIV);

    cfbEncryption.ProcessData((CryptoPP::byte*)bodyBuffer.data.get(),
                              (CryptoPP::byte*)bodyBuffer.data.get(), bodyBuffer.size);

    if (this->nextHandler)
    {
        return this->nextHandler->handleOutcomingMessage(/*message, */bodyBuffer);
    }
    return Utility::MessageProcessingState::SUCCESS;
}

Utility::MessageProcessingState EncryptionHandler::handleIncomingMessageBody(
    yas::shared_buffer buffer/*, Network::Message& message*/)
{
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(mEncryptionKey,
                                                                CryptoPP::AES::BLOCKSIZE, mIV);

    cfbDecryption.ProcessData((CryptoPP::byte*)buffer.data.get(),
                              (CryptoPP::byte*)buffer.data.get(), buffer.size);

    if (this->nextHandler)
    {
        return this->nextHandler->handleIncomingMessageBody(buffer/*, message*/);
    }
    return Utility::MessageProcessingState::SUCCESS;
}

void EncryptionHandler::calculateDigest(const CryptoPP::SecByteBlock& sharedSecret)
{
    CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());
}

void EncryptionHandler::setIVBlock(const std::string& IV)
{
    std::memcpy(mIV, IV.data(), IV.size());
}

const std::string EncryptionHandler::getIVBlock()
{
    return std::string((char*)mIV, CryptoPP::AES::BLOCKSIZE);
}

const void EncryptionHandler::calculateDigestAndGenerateIVBlock(
    const CryptoPP::SecByteBlock& sharedSecret)
{
    CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());

    mRng.GenerateBlock(mIV, CryptoPP::AES::BLOCKSIZE);  // Generate a random IV
}
}  // namespace Utility