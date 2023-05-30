#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;
using Server::Builder::SettingsManager;

TEST_CASE("Constructor of the ServerBuilder [ServerBuilder][Success]")
{
    SECTION("Constructing with parameters")
    {
        CHECK_NOTHROW(ServerBuilder(std::make_unique<SettingsManager>()));
    }
}

TEST_CASE("SetRepoManager of the ServerBuilder [ServerBuilder][Success]")
{
    SECTION("Setting another value")
    {
        ServerBuilder serverBuilder(std::make_unique<SettingsManager>());

        CHECK_NOTHROW(serverBuilder.setRepoManager(nullptr));
    }
}

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

     auto      settingsManager = std::make_unique<SettingsManager>(configArgs.getSettings());
    TestServer testServer      = ServerBuilder(std::move(settingsManager))
                                          .setRepoManager(getTestDatabase())
                                          .makeServer();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadServerPortArguments().second)
            == TestServerInfo::Port::test);
    }
    CHECK_NOTHROW(testServer->start());
}
