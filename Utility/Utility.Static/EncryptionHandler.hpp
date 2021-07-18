#pragma once

#include "Handler.hpp"

#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/sha.h>

namespace Utility
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
    MessageProcessingState handleOutcomingMessage(Network::Message& message, yas::shared_buffer& bodyBuffer) override;
    /**
     * @brief Method for decryption of incoming message bodies.
     * @param buffer - buffer that contains data that should be decrypted.
     * @param messageHeader - variable that will contain decrypted message body.
     */
    MessageProcessingState handleIncomingMessageBody(yas::shared_buffer buffer, Network::Message& message) override;

    static void calculateDigest(const CryptoPP::SecByteBlock& sharedSecret);

    static void setIVBlock(const std::string& IV);

    static const std::string getIVBlock();

    static const void calculateDigestAndGenerateIVBlock(const CryptoPP::SecByteBlock& sharedSecret);
    // Needs improvements in this place~

private:
    inline static CryptoPP::byte mIV[CryptoPP::AES::BLOCKSIZE]{};
    inline static CryptoPP::AutoSeededRandomPool mRng{};
    inline static CryptoPP::SecByteBlock mEncryptionKey{CryptoPP::SHA256::DIGESTSIZE};
};
}  // namespace Network
