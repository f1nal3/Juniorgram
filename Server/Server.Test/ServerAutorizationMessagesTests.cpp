#include <catch2/catch.hpp>
#include "TestUtility.hpp"

using namespace TestUtility;

TEST_CASE("Workflow of checking registration request of server")
{
    //int testArgc = 6;
    //const char* testArgv[] = {"65001", "juniorgram", "127.0.0.1", "5432", "postgres", "123123"};
    ArgParser  parser(__argc, const_cast<const char**>(__argv));
    TestServer server = ServerBuilder()
                                        .setValue(parser.getPair("--serverport"))
                                        .setValue(parser.getPair("--dbname"))
                                        .setValue(parser.getPair("--hostaddr"))
                                        .setValue(parser.getPair("--port"))
                                        .setValue(parser.getPair("--user"))
                                        .setValue(parser.getPair("--password"))
                                        .setValue(getTestingDatabase().release())
                                        .makeServer();
    Client     mockClient;
    server->start();

    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
        == testAcceptingConnection)
    {
        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::RegistrationRequest));
    }
    testServerUpdating(server);
}
//
//TEST_CASE("Workflow of checking login request of server")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(),getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::LoginRequest));
//    }
//    testServerUpdating(serverTest);
//}
