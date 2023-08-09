#pragma once

#include "Network/Handler.hpp"

namespace Network
{
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
                case Message::MessageType::ServiceAccept:
                    break;

                case Message::MessageType::ServicePing:
                    break;

                case Message::MessageType::MessageAll:
                    break;

                case Message::MessageType::ServiceMessage:
                    break;

                case Message::MessageType::ChannelListRequest:
                    state = processOutcomingMessageBody<std::vector<Models::ChannelInfo>>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::MessageInfo>>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageStoreRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringMessageCodes>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ReplyHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ReplyHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::ReplyInfo>>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ReplyStoreRequest:
                    state = processOutcomingMessageBody<Models::ReplyInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ReplyStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringReplyCodes>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageDeleteRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::DeletingMessageCodes>(bodyBuffer, message.mBody);
                    break;
                    
                case Message::MessageType::MessageEditRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageEditAnswer:
                    state = processOutcomingMessageBody<Utility::EditingMessageCodes>(bodyBuffer, message.mBody);
                    break;       

                case Message::MessageType::MessageReactionRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::MessageReactionAnswer:
                    state = processOutcomingMessageBody<Utility::ReactionMessageCodes>(bodyBuffer, message.mBody);
                    break;
                    
                case Message::MessageType::RegistrationRequest:
                    state = processOutcomingMessageBody<Models::RegistrationInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::RegistrationAnswer:
                    state = processOutcomingMessageBody<Utility::RegistrationCodes>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::LoginRequest:
                    state = processOutcomingMessageBody<Models::LoginInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::LoginAnswer:
                    state = processOutcomingMessageBody<bool>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelLeaveRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelLeaveAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelLeaveCodes>(bodyBuffer, message.mBody);
                    break;
                case Message::MessageType::ChannelSubscribeRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelSubscribeAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelSubscribingCodes>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelSubscriptionListRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelSubscriptionListAnswer:
                    state = processOutcomingMessageBody<std::vector<uint64_t>>(bodyBuffer, message.mBody);
                    break;
                case Message::MessageType::ChannelDeleteRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelDeleteCode>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelCreateRequest:
                    state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::ChannelCreateAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelCreateCodes>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::DirectMessageCreateRequest:
                    state = processOutcomingMessageBody<uint64_t>(bodyBuffer, message.mBody);
                    break;

                case Message::MessageType::DirectMessageCreateAnswer:
                    state = processOutcomingMessageBody<Utility::DirectMessageStatus>(bodyBuffer, message.mBody);
                    break;

                default:
                    break;
            }
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleOutcomingMessage(message, bodyBuffer);
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
            case Message::MessageType::ServiceAccept:
                break;
            case Message::MessageType::ServicePing:
                break;
            case Message::MessageType::MessageAll:
                break;
            case Message::MessageType::ServiceMessage:
                break;
            case Message::MessageType::ChannelListRequest:
            {
                state = processIncomingMessageBody<std::vector<Models::ChannelInfo>>(buffer, message);
                break;
            }
            case Message::MessageType::MessageHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(buffer, message);
                break;
            }
            case Message::MessageType::MessageHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::MessageInfo>>(buffer, message);
                break;
            }
            case Message::MessageType::MessageStoreRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case Message::MessageType::MessageStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringMessageCodes>(buffer, message);
                break;
            }
            case Message::MessageType::ReplyHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(buffer, message);
                break;
            }
            case Message::MessageType::ReplyHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::ReplyInfo>>(buffer, message);
                break;
            }
            case Message::MessageType::ReplyStoreRequest:
            {
                state = processIncomingMessageBody<Models::ReplyInfo>(buffer, message);
                break;
            }
            case Message::MessageType::ReplyStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringReplyCodes>(buffer, message);
                break;
            }
            case Message::MessageType::MessageDeleteRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case Message::MessageType::MessageDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::DeletingMessageCodes>(buffer, message);
                break;
            }
            case Message::MessageType::MessageEditRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case Message::MessageType::MessageEditAnswer:
            {
                state = processIncomingMessageBody<Utility::EditingMessageCodes>(buffer, message);
                break;
            }
            case Message::MessageType::MessageReactionRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(buffer, message);
                break;
            }
            case Message::MessageType::MessageReactionAnswer:
            {
                state = processIncomingMessageBody<Utility::ReactionMessageCodes>(buffer, message);
                break;
            }
            case Message::MessageType::RegistrationRequest:
            {
                state = processIncomingMessageBody<Models::RegistrationInfo>(buffer, message);
                break;
            }
            case Message::MessageType::RegistrationAnswer:
            {
                state = processIncomingMessageBody<Utility::RegistrationCodes>(buffer, message);
                break;
            }
            case Message::MessageType::LoginRequest:
            {
                state = processIncomingMessageBody<Models::LoginInfo>(buffer, message);
                break;
            }
            case Message::MessageType::LoginAnswer:
            {
                state = processIncomingMessageBody<bool>(buffer, message);
                break;
            }

            case Message::MessageType::ChannelLeaveRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelLeaveAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelLeaveCodes>(buffer, message);
                break;
            }

            case Message::MessageType::ChannelSubscribeRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelSubscribeAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelSubscribingCodes>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelSubscriptionListRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelSubscriptionListAnswer:
            {
                state = processIncomingMessageBody<std::vector<uint64_t>>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelCreateRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelCreateCodes>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelDeleteRequest:
            {
                state = processIncomingMessageBody<std::string>(buffer, message);
                break;
            }
            case Message::MessageType::ChannelDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelDeleteCode>(buffer, message);
                break;
            }

            case Message::MessageType::DirectMessageCreateRequest:
            {
                state = processIncomingMessageBody<uint64_t>(buffer, message);
                break;
            }

            case Message::MessageType::DirectMessageCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::DirectMessageStatus>(buffer, message);
                break;
            }
            default:
                break;
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleIncomingMessageBody(buffer, message);
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
