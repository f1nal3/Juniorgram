#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

constexpr bool acceptingConnection = true;

TEST_CASE("Check channel create request of server")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());
      
    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelCreateRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel list request from server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelListRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel subscribe request from server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelSubscribeRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel subscription list request from server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelSubscriptionListRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel leave request from server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort())
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelLeaveRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check channel delete request from server side")
{
    Client     Client;
    testServer serverTest(getTestingPort(), getTestingDatabase());

    serverTest.start();

    if (bindOfConnectToServer(Client, getTestingAddress(), getTestingPort()) 
        == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ChannelDeleteRequest));
    }
    testServerUpdating(serverTest);
}
