#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"
#include "Utilities/WaitFor.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Default request procedures [Server][Success]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);

    WaitFor waiter(milliseconds(1000));
    
    SECTION("Successful request to check a ping")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ServerPing, MessageBody::ValidBody);
        
        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() ==
            TestObject::MessageResult::Success);
        testServer->stop();
    }

    SECTION("Successful default request answer")
    {
        Message invalidMessage;
        constexpr auto failedType = int16_t(666);

        const auto& messageInstance = makeMessage(invalidMessage,
            MessageType(failedType), MessageBody::InvalidBody);

        CHECK_NOTHROW(client.send(messageInstance));
        testServer->update();

        waiter.wait();
        REQUIRE(client.getMessageResult().back() ==
            TestObject::MessageResult::Success);
        testServer->stop();
    }
}
