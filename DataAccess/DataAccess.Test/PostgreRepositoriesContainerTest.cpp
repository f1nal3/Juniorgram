#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"

TEST_CASE("PostgreRepositoryContainer", "[dummy]")
{
	using DataAccess::PostgreRepositoryContainer;
	using DataAccess::PostgreAdapter;

	SECTION("Constructor check")
	{
		REQUIRE_NOTHROW(PostgreRepositoryContainer(PostgreAdapter::Instance(DBOptions::test)));
	}
}