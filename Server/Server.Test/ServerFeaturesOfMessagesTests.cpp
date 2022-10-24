#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Check all messages from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageAll));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check message history request of server")
{
    Client Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageHistoryRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check reply message history request of server")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ReplyHistoryRequest));
    }
    testServerUpdating(serverTest);
}

/* TEST_CASE("Check direct message create request from server side")
/  {
/    Client     Client;
/    testServer serverTest(ServerInfo::Port::test);
/    bool       acceptingConnection = true;
/
/    serverTest.start();
/
/    if (bindOfConnectToServer(Client) == acceptingConnection)
/    {
/        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::DirectMessageCreateRequest));
/    }
/    testServerUpdating(serverTest);
/   }
*/

TEST_CASE("Check message store request from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageStoreRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check reply message store request from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::ReplyStoreRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check message reaction request from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageReactionRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check message edit from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageEditRequest));
    }
    testServerUpdating(serverTest);
}

TEST_CASE("Check message delete request from server side")
{
    Client     Client;
    testServer serverTest(ServerInfo::Port::test);
    bool       acceptingConnection = true;

    serverTest.start();

    if (bindOfConnectToServer(Client) == acceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(Client, MessageType::MessageDeleteRequest));
    }
    testServerUpdating(serverTest);
}
