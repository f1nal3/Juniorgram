#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Successfull Server start up [Server][Success]")
{
    auto testServer = makeTestServer();

    SECTION("Comparison of the specified port with the expected port")
    {
        REQUIRE(std::stoi(configArgs.getServerPortArguments().second) 
            == TestServerInfo::Port::test);
    }
    CHECK_NOTHROW(testServer->start());
}

TEST_CASE("Server start up with bad port [Server][Failed]")
{   
    TestServer testServer = ServerBuilder()
                                .setValue(configArgs.getBadServerPortArguments())
                                .setValue(configArgs.getDatabaseArguments())
                                .setValue(configArgs.getHostAddrArguments())
                                .setValue(configArgs.getDatabasePortArguments())
                                .setValue(configArgs.getDatabaseUserArguments())
                                .setValue(configArgs.getDatabasePasswordArguments())
                                .setValue(getTestDatabase().release())
                                .makeServer();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadServerPortArguments().second) 
            == TestServerInfo::Port::test);
    }
    CHECK_NOTHROW(testServer->start());
}
