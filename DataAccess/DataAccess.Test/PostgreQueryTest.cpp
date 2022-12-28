#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreQuery.hpp"
#include "DataBaseOptions.hpp"

using DataAccess::PostgreAdapter;
using DataAccess::PostgreQuery;

TEST_CASE("PostgreQuery", "[dummy]")
{
	SECTION("Default constructor")
	{
		REQUIRE_NOTHROW(PostgreQuery("users"));
	}

	SECTION("No default condition in constructor")
	{
		REQUIRE_NOTHROW(PostgreQuery("users"), PostgreAdapter::Instance(DBOptions::test));
	}
}
