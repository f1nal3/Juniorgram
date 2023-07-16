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
    * @param source - variable that contains data that should be serialized.
    * @param destination - buffer that will contain serialized message body.
    */
    MessageProcessingState handleOutcomingMessage(const Message& source, yas::shared_buffer& destination) override
    {
        SerializedState state = SerializedState::SUCCESS;

        if (source.mBody.has_value())
        {
            switch (source.mHeader.mMessageType)
            {
                case Message::MessageType::ServerAccept:
                    break;

                case Message::MessageType::ServerPing:
                    break;

                case Message::MessageType::MessageAll:
                    break;

                case Message::MessageType::ServerMessage:
                    break;

                case Message::MessageType::ChannelListRequest:
                    state = processOutcomingMessageBody<std::vector<Models::ChannelInfo>>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::MessageInfo>>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageStoreRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringMessageCodes>(destination, source.mBody);
                    break;

                case Message::MessageType::ReplyHistoryRequest:
                    state = processOutcomingMessageBody<std::uint64_t>(destination, source.mBody);
                    break;

                case Message::MessageType::ReplyHistoryAnswer:
                    state = processOutcomingMessageBody<std::vector<Models::ReplyInfo>>(destination, source.mBody);
                    break;

                case Message::MessageType::ReplyStoreRequest:
                    state = processOutcomingMessageBody<Models::ReplyInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::ReplyStoreAnswer:
                    state = processOutcomingMessageBody<Utility::StoringReplyCodes>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageDeleteRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::DeletingMessageCodes>(destination, source.mBody);
                    break;
                    
                case Message::MessageType::MessageEditRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageEditAnswer:
                    state = processOutcomingMessageBody<Utility::EditingMessageCodes>(destination, source.mBody);
                    break;       

                case Message::MessageType::MessageReactionRequest:
                    state = processOutcomingMessageBody<Models::MessageInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::MessageReactionAnswer:
                    state = processOutcomingMessageBody<Utility::ReactionMessageCodes>(destination, source.mBody);
                    break;
                    
                case Message::MessageType::RegistrationRequest:
                    state = processOutcomingMessageBody<Models::RegistrationInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::RegistrationAnswer:
                    state = processOutcomingMessageBody<Utility::RegistrationCodes>(destination, source.mBody);
                    break;

                case Message::MessageType::LoginRequest:
                    state = processOutcomingMessageBody<Models::LoginInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::LoginAnswer:
                    state = processOutcomingMessageBody<bool>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelLeaveRequest:
                    state = processOutcomingMessageBody<std::string>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelLeaveAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelLeaveCodes>(destination, source.mBody);
                    break;
                case Message::MessageType::ChannelSubscribeRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelSubscribeAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelSubscribingCodes>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelSubscriptionListRequest:
                    state = processOutcomingMessageBody<Models::ChannelSubscriptionInfo>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelSubscriptionListAnswer:
                    state = processOutcomingMessageBody<std::vector<uint64_t>>(destination, source.mBody);
                    break;
                case Message::MessageType::ChannelDeleteRequest:
                    state = processOutcomingMessageBody<std::string>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelDeleteAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelDeleteCode>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelCreateRequest:
                    state = processOutcomingMessageBody<std::string>(destination, source.mBody);
                    break;

                case Message::MessageType::ChannelCreateAnswer:
                    state = processOutcomingMessageBody<Utility::ChannelCreateCodes>(destination, source.mBody);
                    break;

                case Message::MessageType::DirectMessageCreateRequest:
                    state = processOutcomingMessageBody<uint64_t>(destination, source.mBody);
                    break;

                case Message::MessageType::DirectMessageCreateAnswer:
                    state = processOutcomingMessageBody<Utility::DirectMessageStatus>(destination, source.mBody);
                    break;

                default:
                    break;
            }
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleOutcomingMessage(source, destination);
            }
            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

    /**
    * @brief Method for deserialization of incoming message bodies.
    * @param source - buffer that contains data that should be deserialized.
    * @param destination - variable that will contain deserialized message body.
    */
    MessageProcessingState handleIncomingMessageBody(const yas::shared_buffer& source, Message& destination) override
    {
        SerializedState state = SerializedState::FAILURE;

        switch (destination.mHeader.mMessageType)
        {
            case Message::MessageType::ServerAccept:
                break;
            case Message::MessageType::ServerPing:
                break;
            case Message::MessageType::MessageAll:
                break;
            case Message::MessageType::ServerMessage:
                break;
            case Message::MessageType::ChannelListRequest:
            {
                state = processIncomingMessageBody<std::vector<Models::ChannelInfo>>(source, destination);
                break;
            }
            case Message::MessageType::MessageHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(source, destination);
                break;
            }
            case Message::MessageType::MessageHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::MessageInfo>>(source, destination);
                break;
            }
            case Message::MessageType::MessageStoreRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(source, destination);
                break;
            }
            case Message::MessageType::MessageStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringMessageCodes>(source, destination);
                break;
            }
            case Message::MessageType::ReplyHistoryRequest:
            {
                state = processIncomingMessageBody<std::uint64_t>(source, destination);
                break;
            }
            case Message::MessageType::ReplyHistoryAnswer:
            {
                state = processIncomingMessageBody<std::vector<Models::ReplyInfo>>(source, destination);
                break;
            }
            case Message::MessageType::ReplyStoreRequest:
            {
                state = processIncomingMessageBody<Models::ReplyInfo>(source, destination);
                break;
            }
            case Message::MessageType::ReplyStoreAnswer:
            {
                state = processIncomingMessageBody<Utility::StoringReplyCodes>(source, destination);
                break;
            }
            case Message::MessageType::MessageDeleteRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(source, destination);
                break;
            }
            case Message::MessageType::MessageDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::DeletingMessageCodes>(source, destination);
                break;
            }
            case Message::MessageType::MessageEditRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(source, destination);
                break;
            }
            case Message::MessageType::MessageEditAnswer:
            {
                state = processIncomingMessageBody<Utility::EditingMessageCodes>(source, destination);
                break;
            }
            case Message::MessageType::MessageReactionRequest:
            {
                state = processIncomingMessageBody<Models::MessageInfo>(source, destination);
                break;
            }
            case Message::MessageType::MessageReactionAnswer:
            {
                state = processIncomingMessageBody<Utility::ReactionMessageCodes>(source, destination);
                break;
            }
            case Message::MessageType::RegistrationRequest:
            {
                state = processIncomingMessageBody<Models::RegistrationInfo>(source, destination);
                break;
            }
            case Message::MessageType::RegistrationAnswer:
            {
                state = processIncomingMessageBody<Utility::RegistrationCodes>(source, destination);
                break;
            }
            case Message::MessageType::LoginRequest:
            {
                state = processIncomingMessageBody<Models::LoginInfo>(source, destination);
                break;
            }
            case Message::MessageType::LoginAnswer:
            {
                state = processIncomingMessageBody<bool>(source, destination);
                break;
            }

            case Message::MessageType::ChannelLeaveRequest:
            {
                state = processIncomingMessageBody<std::string>(source, destination);
                break;
            }
            case Message::MessageType::ChannelLeaveAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelLeaveCodes>(source, destination);
                break;
            }

            case Message::MessageType::ChannelSubscribeRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(source, destination);
                break;
            }
            case Message::MessageType::ChannelSubscribeAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelSubscribingCodes>(source, destination);
                break;
            }
            case Message::MessageType::ChannelSubscriptionListRequest:
            {
                state = processIncomingMessageBody<Models::ChannelSubscriptionInfo>(source, destination);
                break;
            }
            case Message::MessageType::ChannelSubscriptionListAnswer:
            {
                state = processIncomingMessageBody<std::vector<uint64_t>>(source, destination);
                break;
            }
            case Message::MessageType::ChannelCreateRequest:
            {
                state = processIncomingMessageBody<std::string>(source, destination);
                break;
            }
            case Message::MessageType::ChannelCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelCreateCodes>(source, destination);
                break;
            }
            case Message::MessageType::ChannelDeleteRequest:
            {
                state = processIncomingMessageBody<std::string>(source, destination);
                break;
            }
            case Message::MessageType::ChannelDeleteAnswer:
            {
                state = processIncomingMessageBody<Utility::ChannelDeleteCode>(source, destination);
                break;
            }

            case Message::MessageType::DirectMessageCreateRequest:
            {
                state = processIncomingMessageBody<uint64_t>(source, destination);
                break;
            }

            case Message::MessageType::DirectMessageCreateAnswer:
            {
                state = processIncomingMessageBody<Utility::DirectMessageStatus>(source, destination);
                break;
            }
            default:
                break;
        }

        if (state == SerializedState::SUCCESS)
        {
            if (this->nextHandler)
            {
                this->nextHandler->handleIncomingMessageBody(source, destination);
            }
            return MessageProcessingState::SUCCESS;
        }

        return MessageProcessingState::FAILURE;
    }

private:
    template <typename T>
    SerializedState processOutcomingMessageBody(yas::shared_buffer& destination, const std::any& source) const
    {
        try
        {
            return YasSerializer::template serialize<T>(destination, std::any_cast<T>(source));
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
    SerializedState processIncomingMessageBody(const yas::shared_buffer& source, Message& destination) const
    {
        try
        {
            T messageInfo;

            SerializedState state = YasSerializer::template deserialize<T>(source, messageInfo);

            if (state == SerializedState::SUCCESS)
            {
                destination.mBody = std::make_any<T>(messageInfo);
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
}  /// namespace Network
