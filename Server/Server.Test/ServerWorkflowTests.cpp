#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("TestServerStartUp [success case]")
{
    auto testServer = makeTestServer();
    CHECK_NOTHROW(testServer->start());
    testServer->stop();
}

TEST_CASE("TestServerFailStartUpWithBadPort [success case]")
{   
    auto testServer = makeBadTestServer();
    CHECK_NOTHROW(testServer->start());
    testServer->stop();
}
