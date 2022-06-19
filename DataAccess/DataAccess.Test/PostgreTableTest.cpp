#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreTable.hpp"

using DataAccess::PostgreAdapter;
using DataAccess::PostgreTable;

TEST_CASE("PostgreTable", "[dummy]")
{
	SECTION("Default constructor")
	{
		REQUIRE_NOTHROW(PostgreTable("users"));
	}

	SECTION("No default condition in constructor")
	{
		REQUIRE_NOTHROW(PostgreTable("users"), PostgreAdapter::Instance(DBOptions::test));
	}
}
