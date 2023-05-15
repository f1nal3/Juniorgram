#include <catch2/catch.hpp>

#include "ServerBuilder.hpp"

using Server::Builder::ServerBuilder;
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

        CHECK_NOTHROW(serverBuilder.SetRepoManager(nullptr));
    }
}
