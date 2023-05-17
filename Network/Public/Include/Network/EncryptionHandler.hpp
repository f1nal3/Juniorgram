#pragma once

#include "Handler.hpp"
#include "SessionKeyHolder.hpp"
#include "ICryptography.hpp"



namespace Network
{
/** @class EncryptionHandler
 *  @brief handler class for messages encryption.
 */
class EncryptionHandler : public AbstractHandler
{
    std::uint64_t                                _userId;
    std::shared_ptr<Base::Crypto::ICryptography> _cryptoAlgorithm;

public:
    EncryptionHandler(std::shared_ptr<Base::Crypto::ICryptography> cryptoAlgorithm_, std::uint64_t userId_)
        : _userId(userId_), _cryptoAlgorithm(cryptoAlgorithm_)
    {
    };
    /**
     * @brief Method for encryption of outcoming messages.
     * @param message - buffer that contains data that should be encrypted.
     * @param headerBuffer - buffer that will contain encrypted header.
     * @param bodyBuffer - buffer that will contain encrypted body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        EncryptionState encryptionState = EncryptionState::SUCCESS;

        if (message.mBody.has_value())
        {
            switch (message.mHeader.mMessageType)
            {
                case Message::MessageType::ServerAccept:
                    break;

                case Message::MessageType::ServerPing:
                    break;

                case Message::MessageType::MessageAll:
                    break;

                case Message::MessageType::ServerMessage:
                    break;

                case Message::MessageType::ConnectionInfoRequest:
                    break;

                case Message::MessageType::ConnectionInfoAnswer:
                    break;

                case Message::MessageType::LoginRequest:
                    break;

                case Message::MessageType::LoginAnswer:
                    break;

                case Message::MessageType::KeyAgreement:
                    break;

                case Message::MessageType::ChannelListRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageHistoryRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageHistoryAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageStoreRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageStoreAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyHistoryRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyHistoryAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyStoreRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyStoreAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageDeleteRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageDeleteAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageEditRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageEditAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageReactionRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageReactionAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::RegistrationRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::RegistrationAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelLeaveRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelLeaveAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscribeRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscribeAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscriptionListRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscriptionListAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelDeleteRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelDeleteAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelCreateRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelCreateAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::DirectMessageCreateRequest:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::DirectMessageCreateAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::KeyConfirmation:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                case Message::MessageType::KeyConfirmationAnswer:
                    encryptionState = encryptOutcomingMessageBody(bodyBuffer, message.mHeader);
                    break;

                default:
                    break;
            }
        }

        if (encryptionState == EncryptionState::SUCCESS)
        {
            if (this->nextHandler)
            {
                return this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
            }
            return MessageProcessingState::SUCCESS;
        }
        return MessageProcessingState::FAILURE;
    }

    /**
     * @brief Method for decryption of incoming message bodies.
     * @param bodyBuffer - buffer that contains data that should be decrypted.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer bodyBuffer, Message& message) override
    {
        EncryptionState decryptionState = EncryptionState::SUCCESS;

        yas::shared_buffer decryptedBuffer = bodyBuffer;

        if (message.mBody.has_value())
        {
            switch (message.mHeader.mMessageType)
            {
                case Message::MessageType::ServerAccept:
                    break;

                case Message::MessageType::ServerPing:
                    break;

                case Message::MessageType::MessageAll:
                    break;

                case Message::MessageType::ServerMessage:
                    break;

                case Message::MessageType::ConnectionInfoRequest:
                    break;

                case Message::MessageType::ConnectionInfoAnswer:
                    break;

                case Message::MessageType::LoginRequest:
                    break;

                case Message::MessageType::LoginAnswer:
                    break;

                case Message::MessageType::KeyAgreement:
                    break;

                case Message::MessageType::ChannelListRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageHistoryRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageHistoryAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageStoreRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageStoreAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyHistoryRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyHistoryAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyStoreRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ReplyStoreAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageDeleteRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageDeleteAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageEditRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageEditAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageReactionRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::MessageReactionAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::RegistrationRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::RegistrationAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelLeaveRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelLeaveAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscribeRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscribeAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscriptionListRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelSubscriptionListAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelDeleteRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelDeleteAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelCreateRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::ChannelCreateAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::DirectMessageCreateRequest:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::DirectMessageCreateAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::KeyConfirmation:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                case Message::MessageType::KeyConfirmationAnswer:
                    decryptionState = decryptIncomingMessageBody(decryptedBuffer, message.mHeader);
                    break;

                default:
                    break;
            }
        }

        if (decryptionState == EncryptionState::SUCCESS)
        {
            if (this->nextHandler)
            {
                return this->nextHandler->handleIncomingMessageBody(decryptedBuffer, message);
            }
            return MessageProcessingState::SUCCESS;
        }
        return MessageProcessingState::FAILURE;
    }
private:

    EncryptionState encryptOutcomingMessageBody(yas::shared_buffer& buffer, const Message::MessageHeader& msgHeader)
    {
        CryptoPP::SecByteBlock key = Base::SessionKeyHolder::Instance().getKey(_userId);
        CryptoPP::SecByteBlock initVector(reinterpret_cast<const CryptoPP::byte*>(msgHeader.mIv.data()), msgHeader.mIv.size());

        if (!key.empty())
        {
            return _cryptoAlgorithm->encrypt(buffer, key, initVector, msgHeader.mAuthData);
        }
        return EncryptionState::FAILURE;
    }

    EncryptionState decryptIncomingMessageBody(yas::shared_buffer& buffer, const Message::MessageHeader& msgHeader)
    {
        CryptoPP::SecByteBlock key = Base::SessionKeyHolder::Instance().getKey(_userId);
        CryptoPP::SecByteBlock initVector(reinterpret_cast<const CryptoPP::byte*>(msgHeader.mIv.data()), msgHeader.mIv.size());

        if (!key.empty())
        {
            return _cryptoAlgorithm->decrypt(buffer, key, initVector, msgHeader.mAuthData);
        }
        return EncryptionState::FAILURE;
    }
};
}  // namespace Network
