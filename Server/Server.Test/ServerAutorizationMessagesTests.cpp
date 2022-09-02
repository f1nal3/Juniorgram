#include <catch2/catch.hpp>
#include <TestUtility.hpp>

using namespace TestUtility;

TEST_CASE("Check registration request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::RegistrationRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}

TEST_CASE("Check login request of server")
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
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::LoginRequest));
        Client.disconnectFromServer();
    });

    threadMockClient.join();
    threadServer.join();
}
