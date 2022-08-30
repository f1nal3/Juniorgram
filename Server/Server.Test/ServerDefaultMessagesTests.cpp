#include <catch2/catch.hpp>
#include <TestUtility.hpp>

#include <thread>

using namespace TestUtility;

TEST_CASE("Check server ping")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]()
    {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client Client;

    std::thread threadMockClient([&Client]()
    {
        bindOfConnectToServer(Client);
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ServerPing));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check default request of server side")
{
    testServer serverTest(ServerInfo::Port::test);

    std::thread threadServer([&serverTest]()
    {
        serverTest.start();
        testServerUpdating(serverTest);
    });

    Client        Client;
    const int16_t failedType = 666;

    std::thread threadMockClient([&Client, &failedType]()
    {
        bindOfConnectToServer(Client);
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType(failedType)));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
