#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Default request procedures [Server][Success]")
{
    auto testServer = makeTestServer();
    testServer->start();

    TestClient client;
    client.connectToServer(TestServerInfo::Address::local, TestServerInfo::Port::test);
    
    SECTION("Successful request to check a ping")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ServerPing, MessageBody::ValidBody);
        
        client.send(messageInstance);
        testServer->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() ==
             Client::TestObject::MessageResult::Success);
        testServer->stop();
    }
}
