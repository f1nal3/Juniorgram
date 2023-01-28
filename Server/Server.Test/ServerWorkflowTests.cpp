#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow startup server")
{
    Client     mockClient;
    ArgParser  parser(configArgc, configArgv);
    TestServer server = ServerBuilder()
                            .setValue(parser.getPair("--serverport"))
                            .setValue(parser.getPair("--dbname"))
                            .setValue(parser.getPair("--hostaddr"))
                            .setValue(parser.getPair("--port"))
                            .setValue(parser.getPair("--user"))
                            .setValue(parser.getPair("--password"))
                            .setValue(getTestingDatabase().release())
                            .makeServer();
    CHECK_NOTHROW(server->start());
    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
        == testAcceptingConnection)
    {
        bindOfSendingMessage(mockClient, MessageType::ServerAccept);
    }
    REQUIRE_NOTHROW(testServerUpdating(server));
}

suppressWarning(4244, "-Wconversion")
suppressWarning(4242, "-Wconversion")
TEST_CASE("Workflow fail startup of server with bad port")
{    
    uint32_t       testBadPort = 6666666;
    ArgParser  parser(configArgc, configArgv);
    TestServer badServer = ServerBuilder()
                            .setValue(parser.getPair("--serverport"))
                            .setValue(parser.getPair("--dbname"))
                            .setValue(parser.getPair("--hostaddr"))
                            .setValue(parser.getPair("--port"))
                            .setValue(parser.getPair("--user"))
                            .setValue(parser.getPair("--password"))
                            .setValue(getTestingDatabase().release())
                            .makeServer();
    CHECK_NOTHROW(badServer->start());
}
restoreWarning
restoreWarning
