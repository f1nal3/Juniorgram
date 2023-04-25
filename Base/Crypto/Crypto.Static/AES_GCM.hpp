#pragma once

#include <cryptopp/aes.h>
#include <cryptopp/gcm.h>
#include <cryptopp/filters.h>

#include "SessionKeyHolder.hpp"
#include "FileLogger.hpp"
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

/** @class AES_GCM
* @brief Encryption class according to the AES GCM algorithm
* @details Uses key from SessionKeyHolder and initial vector from MessageHeader.
*/
class AES_GCM final : public Base::Crypto::ICryptography
{
public:
    AES_GCM(uint64_t userId_, const std::string& login_) :
        _userID(userId_),
        _authData(Base::Hashing::SHA_256(login_))
    {
    };

    /** @brief Method for encrypting data
    * @details Designed for use in EncryptionHandler
    */
    Utility::GeneralCodes encrypt(const std::string& initVector, yas::shared_buffer& bodyBuffer)
    {
        try
        {
            yas::shared_buffer                  encryptedBody(bodyBuffer.size);
            GCM<AES, GCM_2K_Tables>::Encryption encryptor;

            {
                SecByteBlock key;
                SecByteBlock initVec(reinterpret_cast<const byte*>(initVector.data()), initVector.size());

                if (keyHolder::Instance().getOwner() == Owner::SERVER)
                {
                    key = keyHolder::Instance().getUserKey(_userID);
                    encryptor.SetKeyWithIV(key.begin(), key.size(), initVec.begin(), initVec.size());
                }
                else
                {
                    key = keyHolder::Instance().getKey();
                    encryptor.SetKeyWithIV(key.begin(), key.size(), initVec.begin(), initVec.size());
                }
            }

            AuthenticatedEncryptionFilter authEncFilter(encryptor,
                new ArraySink(reinterpret_cast<byte*>(encryptedBody.data.get()), encryptedBody.size),
                false, TAG_SIZE);

            authEncFilter.ChannelPut(AAD_CHANNEL, reinterpret_cast<const byte*>(_authData.data()), _authData.size());
            authEncFilter.ChannelMessageEnd(AAD_CHANNEL);

            authEncFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(bodyBuffer.data.get()), bodyBuffer.size);
            authEncFilter.ChannelMessageEnd(DEFAULT_CHANNEL);

            bodyBuffer = encryptedBody;
        }
        catch (CryptoPP::Exception& e)
        {
            Base::Logger::FileLogger::getInstance().log(e.what(), Base::Logger::LogLevel::ERR);
            return Utility::GeneralCodes::FAILED;
        }
        return Utility::GeneralCodes::SUCCESS;
    }

    /** @brief Method for decrypting data
     * @details Designed for use in EncryptionHandler
     */
    Utility::GeneralCodes decrypt(yas::shared_buffer& buffer, const std::string& initVector)
    {
        GCM<AES, GCM_2K_Tables>::Decryption decryptor;
        {
            SecByteBlock key;
            SecByteBlock initVec(reinterpret_cast<const byte*>(initVector.data()), initVector.size());

            if (keyHolder::Instance().getOwner() == Owner::SERVER)
            {
                key = keyHolder::Instance().getUserKey(_userID);
                decryptor.SetKeyWithIV(key.begin(), key.size(), initVec.begin(), initVec.size());
            }
            else
            {
                key = keyHolder::Instance().getKey();
                decryptor.SetKeyWithIV(key.begin(), key.size(), initVec.begin(), initVec.size());
            }
        }

        AuthenticatedDecryptionFilter authDecrFilter(decryptor, NULL,
            AuthenticatedDecryptionFilter::Flags::MAC_AT_BEGIN |
            HashVerificationFilter::Flags::THROW_EXCEPTION,
            TAG_SIZE);

        yas::shared_buffer encryptedBody(buffer.data.get(), buffer.size - TAG_SIZE);
        yas::shared_buffer msgAuthCode(buffer.data.get() + buffer.size, TAG_SIZE);

        if (buffer.size != encryptedBody.size + msgAuthCode.size || TAG_SIZE != msgAuthCode.size)
        {
            using Base::Logger::FileLogger;
            using Base::Logger::LogLevel;

            FileLogger::getInstance().log("Message splitting error", LogLevel::ERR);
            return Utility::GeneralCodes::FAILED;
        }

        authDecrFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(msgAuthCode.data.get()), msgAuthCode.size);
        authDecrFilter.ChannelPut(AAD_CHANNEL, reinterpret_cast<const byte*>(_authData.data()), _authData.size());
        authDecrFilter.ChannelPut(DEFAULT_CHANNEL, reinterpret_cast<const byte*>(encryptedBody.data.get()), encryptedBody.size);

        try
        {
            authDecrFilter.ChannelMessageEnd(AAD_CHANNEL);
            authDecrFilter.ChannelMessageEnd(DEFAULT_CHANNEL);
        }
        catch (CryptoPP::Exception& exception)
        {
            Base::Logger::FileLogger::getInstance().log(exception.what(), Base::Logger::LogLevel::ERR);
            return Utility::GeneralCodes::FAILED;
        }
        if (!authDecrFilter.GetLastResult())
        {
            Base::Logger::FileLogger::getInstance().log("Data integrity of message is violated", Base::Logger::LogLevel::ERR);
            return Utility::GeneralCodes::FAILED;

        }
        auto               countBytesForReading = authDecrFilter.MaxRetrievable();
        yas::shared_buffer decryptedBody(countBytesForReading);
        authDecrFilter.Get(reinterpret_cast<byte*>(decryptedBody.data.get()), countBytesForReading);

        buffer = decryptedBody;
        return Utility::GeneralCodes::SUCCESS;
    }

private:
    /** @brief Authentification tag
    * @details Tag with default size. It is not recommended to use a shorter tag,
    * because these are truncated 128-bit (16-bytes) tags.
    */
    static const uint8_t TAG_SIZE = 16;
    std::string          _authData;  // hash of username
    std::uint64_t        _userID;
};
}  // namespace Base::Crypto::Symmetric
