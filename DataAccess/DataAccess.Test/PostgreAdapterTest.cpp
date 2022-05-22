#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"

using DataAccess::PostgreAdapter;

TEST_CASE("PostgreAdapter test", "[dummy]") 
{ 
	SECTION("Checking our near-constructor stuff")
	{
		REQUIRE_THROWS(PostgreAdapter::Instance("somethingwrong"));
		REQUIRE_THROWS(PostgreAdapter::Instance("1_*14_0aV&"));

		REQUIRE_NOTHROW(PostgreAdapter::Instance());
		REQUIRE_NOTHROW(PostgreAdapter::Instance(DBOptions::real));
	}

	SECTION("Check our connection")
	{
		auto ourAdapter= PostgreAdapter::Instance();
		REQUIRE_NOTHROW(ourAdapter.get()->getConnection());

		REQUIRE(ourAdapter.get()!=nullptr);
		REQUIRE(ourAdapter.get()->isConnected());

	}

	SECTION("Check our connection with bad arguments, but it should use default argument")
	{
		auto ourBadAdapter = PostgreAdapter::Instance();

		REQUIRE_NOTHROW(ourBadAdapter.get()->getConnection());
		REQUIRE(ourBadAdapter.get()->isConnected());
		REQUIRE(ourBadAdapter.get()!=nullptr);

		auto dbName = std::string_view{"juniorgram"};
		REQUIRE(ourBadAdapter.get()->getConnection().dbname() == dbName);
	}

	SECTION("Check how properly we close connection")
	{
		auto ourAdapter = PostgreAdapter::Instance();

		REQUIRE_NOTHROW(ourAdapter.get()->closeConnection());
	}

	SECTION("Check our query")
	{
		auto ourAdapter = PostgreAdapter::Instance();
		auto ourQuery = std::string_view{"SELECT * FROM users"};

		REQUIRE_NOTHROW(ourAdapter.get()->query(ourQuery));

		REQUIRE(ourAdapter.get()->query(ourQuery)== std::nullopt);

	}
}