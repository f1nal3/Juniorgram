#include <network.h>

#include <catch2/catch.hpp>

TEST_CASE("client")
{
    SECTION("Connection")
    {
        network::Client c;
        c.connect("127.0.0.1", 60000);

        // LOOOOOL
        REQUIRE_FALSE(c.isConnected());
    }
}
