#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Successfull Server start up [Server][Success]")
{
    auto testServer = makeTestServer();
    CHECK_NOTHROW(testServer->start());
    testServer->stop();
}

TEST_CASE("Successfull Server start up with bad port [Server][Success]")
{   
    // clarification: The server functionality does not provide a connection to the port with a limited value.
    auto testServer = makeBadTestServer();
    CHECK_NOTHROW(testServer->start());
    testServer->stop();
}
