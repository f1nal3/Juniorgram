#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"
#include "DataBaseOptions.hpp"

using DataAccess::PostgreRepositoryContainer;
using DataAccess::PostgreAdapter;

TEST_CASE("PostgreRepositoryContainer", "[dummy]")
{
	SECTION("Constructor check")
	{
		REQUIRE_NOTHROW(PostgreRepositoryContainer(PostgreAdapter::Instance(DBOptions::test)));
	}
}
