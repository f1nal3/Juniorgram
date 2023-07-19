#pragma once

#include "Network/Handler.hpp"

namespace Network
{
    using MessageType = Message::MessageType;

/**
 * @class SerializationHandler
 * @brief handler class for messages serialization.
 */
class SerializationHandler : public AbstractHandler
{
public:
    /**
     * @brief Method for serialization of outcoming messages.
     * @param message - buffer that contains data that should be serialized.
     * @param bodyBuffer - buffer that will contain serialized body.
     */
    MessageProcessingState handleOutcomingMessage(const Message& message, yas::shared_buffer& bodyBuffer) override
    {
        SerializedState state = SerializedState::SUCCESS;

        if (message.mBody.has_value())
        {
            switch (message.mHeader.mMessageType)
            {
                case MessageType::ServerAccept:
                    break;

                case MessageType::ServerPing:
                    break;

                case MessageType::MessageAll:
                    break;

                case MessageType::ServerMessage:
                    break;

                case MessageType::ConnectionInfoRequest:
                    break;

                case MessageType::ChannelListRequest:
                    state = processOutcomingMessageBody<std::vector<Models::ChannelInfo>>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::MessageInfo>>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageStoreRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringMessageCodes>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ReplyHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ReplyHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::ReplyInfo>>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ReplyStoreRequest:
                    state = processOutcomingMessageBody<Models::ReplyInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ReplyStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringReplyCodes>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageDeleteRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::DeletingMessageCodes>(bodyBuffer, message.mBody);
                    break;
                    
                case MessageType::MessageEditRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageEditAnswer:
                    state = processOutcomingMessageBody<Utility::EditingMessageCodes>(bodyBuffer, message.mBody);
                    break;       

                case MessageType::MessageReactionRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::MessageReactionAnswer:
                    state = processOutcomingMessageBody<Utility::ReactionMessageCodes>(bodyBuffer, message.mBody);
                    break;
                    
                case MessageType::RegistrationRequest:
                    state = processOutcomingMessageBody<Models::RegistrationInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::RegistrationAnswer:
                    state = processOutcomingMessageBody<Utility::RegistrationCodes>(bodyBuffer, message.mBody);
                    break;

                case MessageType::LoginRequest:
                    state = processOutcomingMessageBody<Models::LoginInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::LoginAnswer:
                    state = processOutcomingMessageBody<bool>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelLeaveRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelLeaveAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelLeaveCodes>(bodyBuffer, message.mBody);
                    break;
                case MessageType::ChannelSubscribeRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelSubscribeAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelSubscribingCodes>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelSubscriptionListRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelSubscriptionListAnswer:
                    state = processOutcomingMessageBody<std::vector<uint64_t>>(bodyBuffer, message.mBody);
                    break;
                case MessageType::ChannelDeleteRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelDeleteCode>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelCreateRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ChannelCreateAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelCreateCodes>(bodyBuffer, message.mBody);
                    break;

                case MessageType::DirectMessageCreateRequest:
                    state = processOutcomingMessageBody<uint64_t>(bodyBuffer, message.mBody);
                    break;

                case MessageType::DirectMessageCreateAnswer:
                    state = processOutcomingMessageBody<Utility::DirectMessageStatus>(bodyBuffer, message.mBody);
                    break;

                case MessageType::ConnectionInfoAnswer:
                    state = processOutcomingMessageBody<Models::ConnectionInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::KeyAgreement:
                    state = processOutcomingMessageBody<Models::KeyAgreementInfo>(bodyBuffer, message.mBody);
                    break;

                case MessageType::KeyConfirmation:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case MessageType::KeyConfirmationAnswer:
                    state = processOutcomingMessageBody<bool>(bodyBuffer, message.mBody);
                    break;

                default:
                    break;
            }
        }

        if (state == SerializedState::SUCCESS)
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
     * @brief Method for deserialization of incoming message bodies.
     * @param buffer - buffer that contains data that should be deserialized.
     * @param messageHeader - variable that will contain deserialized message body.
     */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer buffer, Message& message) override
    {
        SerializedState state = SerializedState::FAILURE;
        switch (message.mHeader.mMessageType)
        {
            case MessageType::ServerAccept:
                break;
            case MessageType::ServerPing:
                break;
            case MessageType::MessageAll:
                break;
            case MessageType::ServerMessage:
                break;
            case MessageType::ConnectionInfoRequest:
                break;
            case MessageType::ChannelListRequest:
            {
                state = processIncomingMessageBody<std::vector<Models::ChannelInfo>>(buffer, message);
                break;
            }
            case MessageType::MessageHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(buffer, message);
                break;
            }
            case MessageType::MessageHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::MessageInfo>>(buffer, message);
                break;
            }
            case MessageType::MessageStoreRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case MessageType::MessageStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringMessageCodes>(buffer, message);
                break;
            }
            case MessageType::ReplyHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(buffer, message);
                break;
            }
            case MessageType::ReplyHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::ReplyInfo>>(buffer, message);
                break;
            }
            case MessageType::ReplyStoreRequest:
            {
                state = processIncomingMessageBody<Models::ReplyInfo>(buffer, message);
                break;
            }
            case MessageType::ReplyStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringReplyCodes>(buffer, message);
                break;
            }
            case MessageType::MessageDeleteRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case MessageType::MessageDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::DeletingMessageCodes>(buffer, message);
                break;
            }
            case MessageType::MessageEditRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case MessageType::MessageEditAnswer:
            {
                state = processIncomingMessageBody<Utility::EditingMessageCodes>(buffer, message);
                break;
            }
            case MessageType::MessageReactionRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case MessageType::MessageReactionAnswer:
            {
                state = processIncomingMessageBody<Utility::ReactionMessageCodes>(buffer, message);
                break;
            }
            case MessageType::RegistrationRequest:
            {
                state = processIncomingMessageBody<Models::RegistrationInfo>(buffer, message);
                break;
            }
            case MessageType::RegistrationAnswer:
            {
                state = processIncomingMessageBody<Utility::RegistrationCodes>(buffer, message);
                break;
            }
            case MessageType::LoginRequest:
            {
                state = processIncomingMessageBody<Models::LoginInfo>(buffer, message);
                break;
            }
            case MessageType::LoginAnswer:
            {
                state = processIncomingMessageBody<bool>(buffer, message);
                break;
            }

            case MessageType::ChannelLeaveRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case MessageType::ChannelLeaveAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelLeaveCodes>(buffer, message);
                break;
            }

            case MessageType::ChannelSubscribeRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(buffer, message);
                break;
            }
            case MessageType::ChannelSubscribeAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelSubscribingCodes>(buffer, message);
                break;
            }
            case MessageType::ChannelSubscriptionListRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(buffer, message);
                break;
            }
            case MessageType::ChannelSubscriptionListAnswer:
            {
                state = processIncomingMessageBody<std::vector<uint64_t>>(buffer, message);
                break;
            }
            case MessageType::ChannelCreateRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case MessageType::ChannelCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelCreateCodes>(buffer, message);
                break;
            }
            case MessageType::ChannelDeleteRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case MessageType::ChannelDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelDeleteCode>(buffer, message);
                break;
            }

            case MessageType::DirectMessageCreateRequest:
            {
                state = processIncomingMessageBody<uint64_t>(buffer, message);
                break;
            }

            case MessageType::DirectMessageCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::DirectMessageStatus>(buffer, message);
                break;
            }

            case MessageType::ConnectionInfoAnswer:
            {
                state = processIncomingMessageBody<Models::ConnectionInfo>(buffer, message);
                break;
            }

            case MessageType::KeyAgreement:
            {
                state = processIncomingMessageBody<Models::KeyAgreementInfo>(buffer, message);
                break;
            }
            case MessageType::KeyConfirmation:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }

            case MessageType::KeyConfirmationAnswer:
            {
                state = processIncomingMessageBody<bool>(buffer, message);
                break;
            }
            default:
                break;
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                return this->nextHandler->handleIncomingMessageBody(buffer, message);
            }
            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

private:
    template <typename T>
    SerializedState processOutcomingMessageBody(yas::shared_buffer& bodyBuffer, const std::any messageBody)
    {
        try
        {
            return YasSerializer::template serialize<T>(bodyBuffer, std::any_cast<T>(messageBody));
        }
        catch (const std::bad_any_cast& e)
        {
            Base::Logger::FileLogger::getInstance().log
            (
                std::string(e.what()) + "\nMessage body can't be serialized\n",
                Base::Logger::LogLevel::ERR
            );
            return SerializedState::FAILURE;
        }
    }

    template <typename T>
    SerializedState processIncomingMessageBody(const yas::shared_buffer& bodyBuffer, Message& message)
    {
        try
        {
            T messageInfo;

            SerializedState state = YasSerializer::template deserialize<T>(bodyBuffer, messageInfo);

            if (state == SerializedState::SUCCESS)
            {
                message.mBody = std::make_any<T>(messageInfo);
            }

            return state;
        }
        catch (const std::bad_any_cast& e)
        {
            Base::Logger::FileLogger::getInstance().log
            (
                std::string(e.what()) + "\nMessage body can't be deserialized\n",
                Base::Logger::LogLevel::ERR
            );
            return SerializedState::FAILURE;
        }
    }
};
}  // namespace Network
