#include <catch2/catch.hpp>

#include "DataAccess/QueryBuilder.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"

using DataAccess::AbstractQueryBuilder;
using DataAccess::IAdapter;

TEST_CASE("QueryBuilder cover", "[dummy]")
{
	AbstractQueryBuilder<pqxx::result> testQueryBuilder(Utility::DatabaseType::DB_POSTGRE, "users",
								IAdapter::getInstance<DataAccess::PostgreAdapter>(DBOptions::test));

	SECTION("Try to initialize the reference and catch the exception")
	{
		testQueryBuilder.Insert()->execute();
		REQUIRE_NOTHROW(testQueryBuilder.changeTable("channels"));
		REQUIRE_NOTHROW(testQueryBuilder.Insert());
		REQUIRE_THROWS(testQueryBuilder.Select());
		REQUIRE_THROWS(testQueryBuilder.Update());
		REQUIRE_THROWS(testQueryBuilder.Delete());
		REQUIRE_NOTHROW(testQueryBuilder.changeTable("users"));
	}

	SECTION("Cover of 'join' method")
	{
		auto badJoinType{ 25 };

		REQUIRE_NOTHROW(testQueryBuilder.Select()->join(Utility::SQLJoinType::J_FULL, "channels", "id"));
		REQUIRE_NOTHROW(testQueryBuilder.Select()->join(Utility::SQLJoinType::J_LEFT, "channels", "id"));
		REQUIRE_NOTHROW(testQueryBuilder.Select()->join(Utility::SQLJoinType::J_RIGHT, "channels", "id"));
		REQUIRE_THROWS(testQueryBuilder.Select()->join(std::any_cast<Utility::SQLJoinType>(badJoinType), "users", "login"));
	}
}
 
