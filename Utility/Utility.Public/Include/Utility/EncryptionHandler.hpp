#pragma once
#include "Handler.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>

namespace Network
{
/** @class EncryptionHandler
 *  @brief handler class for messages encryption.
 */
// Needs improvements in this place
class EncryptionHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for encryption of outcoming messages.
     * @param message - buffer that contains data that should be encrypted.
     * @param headerBuffer - buffer that will contain encrypted header.
     * @param bodyBuffer - buffer that will contain encrypted body.
     */
    MessageProcessingState handleOutcomingMessage(Message& message,
                                             yas::shared_buffer& bodyBuffer) override
    {
        // Message::MessageHeader messageHeader = message.mHeader;
        // body encryption
        // messageHeader.mBodySize = static_cast<uint32_t>(bodyBuffer.size);
        // header encryption
        CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption cfbEncryption(mEncryptionKey, CryptoPP::AES::BLOCKSIZE, mIV);
       
        cfbEncryption.ProcessData((CryptoPP::byte*)bodyBuffer.data.get(),
                                  (CryptoPP::byte*)bodyBuffer.data.get(),
                                  bodyBuffer.size);

        if (this->nextHandler)
        {
            return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }

    /**
     * @brief Method for decryption of incoming message bodies.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer,
                                                 Message& message) override
    {
        CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption cfbDecryption(mEncryptionKey, CryptoPP::AES::BLOCKSIZE, mIV);
       
        cfbDecryption.ProcessData((CryptoPP::byte*)buffer.data.get(), (CryptoPP::byte*)buffer.data.get(), buffer.size);

        if (this->nextHandler)
        {
          return this->nextHandler->handleIncomingMessageBody(buffer, message);
        }
        return MessageProcessingState::SUCCESS;
    }

    inline static void calculateDigest(const CryptoPP::SecByteBlock& sharedSecret)
    {
        CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());
    }

    inline static void setIVBlock(const std::string& IV)
    {
        std::memcpy(mIV, IV.data(), IV.size());
    }

    inline static const std::string getIVBlock()
    {
        return std::string((char*)mIV, CryptoPP::AES::BLOCKSIZE);
    }

    inline static const void calculateDigestAndGenerateIVBlock(const CryptoPP::SecByteBlock& sharedSecret)
    {
        CryptoPP::SHA256().CalculateDigest(mEncryptionKey, sharedSecret, sharedSecret.size());

        mRng.GenerateBlock(mIV, CryptoPP::AES::BLOCKSIZE);  // Generate a random IV
    }
    // Needs improvements in this place~

private:
    inline static CryptoPP::byte mIV[CryptoPP::AES::BLOCKSIZE]{};
    inline static CryptoPP::AutoSeededRandomPool mRng{};
    inline static CryptoPP::SecByteBlock mEncryptionKey{CryptoPP::SHA256::DIGESTSIZE};
};
}  // namespace Network
