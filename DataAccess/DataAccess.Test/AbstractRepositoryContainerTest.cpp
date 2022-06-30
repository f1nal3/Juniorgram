#include <catch2/catch.hpp>

#include "DataAccess/AbstractRepositoryContainer.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

using DataAccess::PostgreAdapter;

TEST_CASE("AbstractRepositoryContainer test","[dummy]")
{
	DataAccess::AbstractRepositoryContainer testContainer(PostgreAdapter::Instance());

	SECTION("Register method")
	{
		SECTION("Registering repository")
		{
			REQUIRE_NOTHROW(testContainer.registerRepository<DataAccess::ILoginRepository, DataAccess::LoginRepository >());
		}
	}

	SECTION("GetRepository method")
	{
		SECTION("Let's catch exception")
		{
			REQUIRE_THROWS(testContainer.getRepository<DataAccess::ILoginRepository>());
		}
	}

}
