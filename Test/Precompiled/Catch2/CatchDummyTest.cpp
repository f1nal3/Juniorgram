#include <catch2/catch.hpp>

TEST_CASE("CATCH DUMMY TEST", "[TRUE != FALSE]")
{
	REQUIRE(true != false);
}
