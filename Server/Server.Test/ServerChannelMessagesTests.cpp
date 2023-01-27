//#include <catch2/catch.hpp>
//#include "TestUtility.hpp"
//
//using namespace TestUtility;
//
//TEST_CASE("Workflow of checking channel create request of server")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//      
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelCreateRequest));
//    }
//    testServerUpdating(serverTest);
//}
//
//TEST_CASE("Workflow of checking channel list request from server side")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelListRequest));
//    }
//    testServerUpdating(serverTest);
//}
//
//TEST_CASE("Workflow of checking channel subscribe request from server side")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscribeRequest));
//    }
//    testServerUpdating(serverTest);
//}
//
//TEST_CASE("Workflow of checking channel subscription list request from server side")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelSubscriptionListRequest));
//    }
//    testServerUpdating(serverTest);
//}
//
//TEST_CASE("Workflow of checking channel leave request from server side")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort())
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelLeaveRequest));
//    }
//    testServerUpdating(serverTest);
//}
//
//TEST_CASE("Workflow of checking channel delete request from server side")
//{
//    Client     mockClient;
//    testServer serverTest(getTestingPort(), getTestingDatabase());
//
//    serverTest.start();
//
//    if (bindOfConnectToServer(mockClient, getTestingAddress(), getTestingPort()) 
//        == testAcceptingConnection)
//    {
//        CHECK_NOTHROW(bindOfSendingMessage(mockClient, MessageType::ChannelDeleteRequest));
//    }
//    testServerUpdating(serverTest);
//}
