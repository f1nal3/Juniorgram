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
    Settings settings;

    settings.SetServerPort(configArgs.getBadServerPortArguments().second);
    settings.SetDBName(configArgs.getDatabaseArguments().second);
    settings.SetHostAddress(configArgs.getHostAddrArguments().second);
    settings.SetDBPort(configArgs.getDatabasePortArguments().second);
    settings.SetDBUser(configArgs.getDatabaseUserArguments().second);
    settings.SetDBPassword(configArgs.getDatabasePasswordArguments().second);
    settings.SetRepoManager(getTestDatabase().release());

    TestServer testServer = ServerBuilder(SettingsManager(settings)).makeServer();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadServerPortArguments().second) 
            == TestServerInfo::Port::test);
    }
    CHECK_NOTHROW(testServer->start());
}
