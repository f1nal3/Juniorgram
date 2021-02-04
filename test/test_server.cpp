#include <network.h>

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

TEST_CASE("server")
{
    SECTION("Starting...")
    {
        network::Server s(60000);
        s.start();
        // LOOOOOL
        REQUIRE_FALSE(false);
    }
}
