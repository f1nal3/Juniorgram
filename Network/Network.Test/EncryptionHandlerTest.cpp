#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/YasSerializer.hpp"
TEST_CASE("EncryptionHandler Test")
{
    yas::shared_buffer            bodyBuffer;
    Network::Message              message;
    SECTION("test method handleOutcomingMessage")
    {
        Network::EncryptionHandler   encryptionHandler;
        REQUIRE(Network::MessageProcessingState::FAILURE == encryptionHandler.handleOutcomingMessage(message,bodyBuffer));
        encryptionHandler.setNext(new Network::EncryptionHandler());
        REQUIRE(Network::MessageProcessingState::SUCCESS == encryptionHandler.handleOutcomingMessage(message,bodyBuffer));
        
    }
    SECTION("test method handleIncomingMessageBody")
    {
        Network::EncryptionHandler   encryptionHandler;
        REQUIRE(Network::MessageProcessingState::FAILURE == encryptionHandler.handleIncomingMessageBody(bodyBuffer,message));
        encryptionHandler.setNext(new Network::EncryptionHandler());
        REQUIRE(Network::MessageProcessingState::SUCCESS == encryptionHandler.handleIncomingMessageBody(bodyBuffer,message));
    }
}