#pragma once

#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>

#include "Hashing.hpp"
#include "ICryptography.hpp"

namespace Base::Crypto::Symmetric
{
    using keyHolder = Base::SessionKeyHolder;

    using CryptoPP::AES;
    using CryptoPP::GCM;
    using CryptoPP::GCM_2K_Tables;
    using CryptoPP::AuthenticatedEncryptionFilter;
    using CryptoPP::AuthenticatedDecryptionFilter;
    using CryptoPP::ArraySink;
    using CryptoPP::byte;
    using CryptoPP::HashVerificationFilter;
    using CryptoPP::AAD_CHANNEL;
    using CryptoPP::DEFAULT_CHANNEL;
    using Base::Logger::FileLogger;
    using Base::Logger::LogLevel;

/** @class AES_GCM
* @brief Encryption class according to the AES GCM algorithm
* @details Uses key; uses initialization vector and authentification data from MessageHeader.
*/
class AES_GCM final : public Base::Crypto::ICryptography
{
public:

    /// @brief Method for encryption
    EncryptionState encrypt(yas::shared_buffer& bodyBuffer,
                            const SecByteBlock& key,
                            const SecByteBlock& initVector,
                            const std::string& authData)
    {
        try
        {
            yas::shared_buffer                  encryptedBody(bodyBuffer.size + TAG_SIZE);
            GCM<AES, GCM_2K_Tables>::Encryption encryptor;

            encryptor.SetKeyWithIV(key.data(), key.size(), initVector.data(), initVector.size());

            AuthenticatedEncryptionFilter authEncFilter(encryptor,
                                                        new ArraySink(reinterpret_cast<byte*>(encryptedBody.data.get()), encryptedBody.size),
                                                        false, TAG_SIZE);

            authEncFilter.ChannelPut(AAD_CHANNEL, reinterpret_cast<const byte*>(authData.data()), authData.size());
            authEncFilter.ChannelMessageEnd(AAD_CHANNEL);

            authEncFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(bodyBuffer.data.get()), bodyBuffer.size);
            authEncFilter.ChannelMessageEnd(DEFAULT_CHANNEL);

            bodyBuffer = encryptedBody;
        }
        catch (CryptoPP::Exception& exception)
        {
            FileLogger::getInstance().log(exception.what(), LogLevel::ERR);
            return EncryptionState::FAILURE;
        }
        return EncryptionState::SUCCESS;
    }

    /// @brief Method for decryption
    EncryptionState decrypt(yas::shared_buffer& buffer,
                            const SecByteBlock& key,
                            const SecByteBlock& initVector,
                            const std::string& authData)
    {
        GCM<AES, GCM_2K_Tables>::Decryption decryptor;
        decryptor.SetKeyWithIV(key.data(), key.size(), initVector.data(), initVector.size());

        AuthenticatedDecryptionFilter authDecrFilter(decryptor, NULL,
            AuthenticatedDecryptionFilter::Flags::MAC_AT_BEGIN |
            HashVerificationFilter::Flags::THROW_EXCEPTION,
            TAG_SIZE);

        yas::shared_buffer encryptedBody(buffer.data.get(), buffer.size - TAG_SIZE);
        yas::shared_buffer msgAuthCode(buffer.data.get() + buffer.size - TAG_SIZE, TAG_SIZE);

        if (buffer.size != encryptedBody.size + msgAuthCode.size || TAG_SIZE != msgAuthCode.size)
        {
            FileLogger::getInstance().log("Message splitting error", LogLevel::ERR);
            return EncryptionState::FAILURE;
        }

        authDecrFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(msgAuthCode.data.get()), msgAuthCode.size);
        authDecrFilter.ChannelPut(AAD_CHANNEL, reinterpret_cast<const byte*>(authData.data()), authData.size());
        authDecrFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(encryptedBody.data.get()), encryptedBody.size);

        try
        {
            authDecrFilter.ChannelMessageEnd(AAD_CHANNEL);
            authDecrFilter.ChannelMessageEnd(DEFAULT_CHANNEL);
        }
        catch (CryptoPP::Exception& exception)
        {
            FileLogger::getInstance().log(exception.what(), LogLevel::ERR);
            return EncryptionState::FAILURE;
        }
        if (!authDecrFilter.GetLastResult())
        {
            FileLogger::getInstance().log("Data integrity of message is violated", LogLevel::ERR);
            return EncryptionState::FAILURE;
        }
        auto               countBytesForReading = authDecrFilter.MaxRetrievable();
        yas::shared_buffer decryptedBody(countBytesForReading);
        authDecrFilter.Get(reinterpret_cast<byte*>(decryptedBody.data.get()), countBytesForReading);

        buffer = decryptedBody;
        return EncryptionState::SUCCESS;
    }

private:
    /** @brief Authentification tag
    * @details Tag with default size. It is not recommended to use a shorter tag,
    * because these are truncated 128-bit (16-bytes) tags.
    */
    static const uint8_t TAG_SIZE = 16;
};
}  // namespace Base::Crypto::Symmetric
