#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

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
    using Server::Builder::SettingsManager;

    TestServer testServer = ServerBuilder(std::make_unique<SettingsManager>(configArgs.getSettings()))                                
                                          .SetRepoManager(getTestDatabase().release())
                                          .MakeServer();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadServerPortArguments().second)
            == TestServerInfo::Port::test);
    }
    CHECK_NOTHROW(testServer->start());
}
