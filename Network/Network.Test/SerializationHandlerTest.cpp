#include <catch2/catch.hpp>

#include "Network/Handler.hpp"
#include "Network/Message.hpp"
#include "Network/SerializationHandler.hpp"
#include "Network/YasSerializer.hpp"



TEST_CASE("serializationHandler test"){

    Network::SerializationHandler handler;
    yas::shared_buffer bodyBuffer;
    Network::Message message;
    message.mBody.emplace<std::string>("hi there!");

    SECTION("test method handleOutcomingMessage"){
        message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageAll;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelLeaveAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscribeAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscribeRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerPing;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageDeleteRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageDeleteAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageEditAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageEditRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyHistoryRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyStoreAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::RegistrationRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::RegistrationAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::LoginRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::LoginAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::DirectMessageCreateAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::DirectMessageCreateRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageReactionAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageReactionRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleOutcomingMessage(message, bodyBuffer));


    }

    SECTION("test method handleIncomingMessageBody"){
        message.mHeader.mMessageType = Network::Message::MessageType::ServerAccept;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageAll;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelCreateRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelDeleteRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelLeaveAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscribeAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscribeRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelSubscriptionListRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerPing;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ServerMessage;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ChannelListRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageDeleteRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageDeleteAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageEditAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageEditRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyHistoryRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyStoreAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::ReplyStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageHistoryAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageStoreRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::RegistrationRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::RegistrationAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::LoginRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::LoginAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::DirectMessageCreateAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::DirectMessageCreateRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageReactionAnswer;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));

        message.mHeader.mMessageType = Network::Message::MessageType::MessageReactionRequest;
        REQUIRE(Network::MessageProcessingState::SUCCESS == handler.handleIncomingMessageBody( bodyBuffer, message));
    }

}