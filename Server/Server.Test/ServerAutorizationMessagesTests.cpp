#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerRegistrationRequest [success case]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage, 
        MessageType::RegistrationRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto regInfo = std::any_cast<RegistrationInfo>(validMessage.mBody);
    REQUIRE(regInfo._login == testLogin);

    testServer->update();
    testServer->stop();
}

TEST_CASE("TestServerLogInRequest [success case]")
{
    auto       testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test); 

    Message validMessage;
    auto    messageInstance = makeMessage(validMessage,
        MessageType::LoginRequest, MessageBody::ValidBody);
    CHECK_NOTHROW(client.send(messageInstance));

    auto loginInfo = std::any_cast<LoginInfo>(validMessage.mBody);
    REQUIRE(loginInfo._login == testLogin);
    
    testServer->update();
    testServer->stop();
}
