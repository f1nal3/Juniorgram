#include <catch2/catch.hpp>

#include "DataAccess/QueryBuilder.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"

using DataAccess::QueryBuilder;
using DataAccess::IAdapter;

TEST_CASE("QueryBuilder cover", "[dummy]")
{
	QueryBuilder<pqxx::result> testQueryBuilder(Utility::DatabaseType::DB_POSTGRE, "users",
								IAdapter::getInstance<DataAccess::PostgreAdapter>(DBOptions::real));

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
}
