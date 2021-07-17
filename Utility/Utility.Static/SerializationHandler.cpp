#include "SerializationHandler.hpp"

namespace Utility
{
MessageProcessingState SerializationHandler::handleOutcomingMessage(/*Network::Message& message,*/
                                                                        yas::shared_buffer& bodyBuffer)
{
    SerializedState state = SerializedState::SUCCESS;

    //switch (message.mHeader.mMessageType)
    //{
    //    case Network::Message::MessageType::ServerAccept:
    //        break;
    //    case Network::Message::MessageType::ServerPing:
    //        break;
    //    case Network::Message::MessageType::MessageAll:
    //        break;
    //    case Network::Message::MessageType::ServerMessage:
    //        break;
    //    case Network::Message::MessageType::ChannelListRequest:
    //        state =
    //            processOutcomingMessageBody<std::vector<Network::ChannelInfo>>(bodyBuffer, message.mBody);
    //        break;
    //    case Network::Message::MessageType::MessageHistoryRequest:
    //        state = processOutcomingMessageBody<std::vector<Network::MessageInfo>>(bodyBuffer,
    //                                                                               message.mBody);
    //        break;
    //    case Network::Message::MessageType::MessageStoreRequest:
    //        state = processOutcomingMessageBody<Network::MessageInfo>(bodyBuffer, message.mBody);
    //        break;
    //    case Network::Message::MessageType::RegistrationRequest:
    //        state =
    //            processOutcomingMessageBody<Network::RegistrationInfo>(bodyBuffer, message.mBody);
    //        break;
    //    case Network::Message::MessageType::RegistrationAnswer:
    //        /*my part!!!*/
    //        break;
    //    case Network::Message::MessageType::SendIV:
    //        state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
    //        break;
    //    case Network::Message::MessageType::RegistrationRequestToClient:
    //        state =
    //            processOutcomingMessageBody<Utility::RegistrationCodes>(bodyBuffer, message.mBody);
    //        break;
    //    case Network::Message::MessageType::SetEncryptedConnection:
    //        state = processOutcomingMessageBody<std::string>(bodyBuffer, message.mBody);
    //        break;
    //    default:
    //        break;
    //}

    if (state == SerializedState::SUCCESS)
    {
        if (this->nextHandler)
        {
            return this->nextHandler->handleOutcomingMessage(/*message,*/ bodyBuffer);
        }
        return MessageProcessingState::SUCCESS;
    }
    return MessageProcessingState::FAILURE;
    }

    MessageProcessingState SerializationHandler::handleIncomingMessageBody(yas::shared_buffer buffer/*, Network::Message& message*/)
    {
    SerializedState state = SerializedState::SUCCESS;

    //EXPERIMENTAL
    state;

    //switch (message.mHeader.mMessageType)
    //{
    //    case Network::Message::MessageType::ServerAccept:
    //        break;
    //    case Network::Message::MessageType::ServerPing:
    //        break;
    //    case Network::Message::MessageType::MessageAll:
    //        break;
    //    case Network::Message::MessageType::ServerMessage:
    //        break;
    //    case Network::Message::MessageType::ChannelListRequest:
    //        state = processIncomingMessageBody<std::vector<Network::ChannelInfo>>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::MessageHistoryRequest:
    //        state = processIncomingMessageBody<std::vector<Network::MessageInfo>>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::MessageStoreRequest:
    //        state = processIncomingMessageBody<Network::MessageInfo>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::RegistrationRequest:
    //        state = processIncomingMessageBody<Network::RegistrationInfo>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::RegistrationAnswer:
    //        /*my part!!!*/
    //        break;
    //    case Network::Message::MessageType::RegistrationRequestToClient:
    //        state = processIncomingMessageBody<Utility::RegistrationCodes>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::SendIV:
    //        state = processIncomingMessageBody<std::string>(buffer, message);
    //        break;
    //    case Network::Message::MessageType::SetEncryptedConnection:
    //        state = processIncomingMessageBody<std::string>(buffer, message);
    //        break;
    //    default:
    //        break;
    //}

    if (state == SerializedState::SUCCESS)
    {
    if (this->nextHandler)
    {
        return this->nextHandler->handleIncomingMessageBody(buffer/*, message*/);
    }
    return MessageProcessingState::SUCCESS;
    }
    return MessageProcessingState::FAILURE;
    }

    template <typename T>
    SerializedState SerializationHandler::processOutcomingMessageBody(yas::shared_buffer& bodyBuffer,const std::any messageBody)
    {
        try
        {
            return YasSerializer::template serialize<T>(bodyBuffer, std::any_cast<T>(messageBody));
        }
        catch (const std::bad_any_cast& e)
        {
            std::cout << e.what() << '\n';
            std::cout << "Message body cann't be serialized\n";

            return SerializedState::FAILURE;
        }
    }

    template <typename T>
    SerializedState SerializationHandler::processIncomingMessageBody(yas::shared_buffer& bodyBuffer/*,
                                                                     Network::Message& message*/)
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
            std::cout << e.what() << '\n';
            std::cout << "Message body cann't be deserialized\n";

            return SerializedState::FAILURE;
        }
    }
    }  // namespace Utility