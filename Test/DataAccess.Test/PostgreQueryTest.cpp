#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreQuery.hpp"
#include "DataBaseOptions.hpp"

using DataAccess::PostgreAdapter;
using DataAccess::PGQueryBuilder;

TEST_CASE("PGQueryBuilder", "[dummy]")
{
	SECTION("Default constructor")
	{
		REQUIRE_NOTHROW(PGQueryBuilder("users"));
	}

	SECTION("No default condition in constructor")
	{
		REQUIRE_NOTHROW(PGQueryBuilder("users"), PostgreAdapter::Instance(DBOptions::test));
	}
}
