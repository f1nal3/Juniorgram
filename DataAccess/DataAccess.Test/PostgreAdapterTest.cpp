#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"

using DataAccess::PostgreAdapter;

TEST_CASE("PostgreAdapter test", "[dummy]") 
{ 
	SECTION("Checking our near-constructor stuff")
	{
		REQUIRE_NOTHROW(PostgreAdapter::Instance());
		REQUIRE_NOTHROW(PostgreAdapter::Instance(DBOptions::real));
		REQUIRE_NOTHROW(PostgreAdapter::Instance(""));
	}

	auto ourAdapter = PostgreAdapter::Instance(DBOptions::test);

	SECTION("Check our connection")
	{
		REQUIRE_NOTHROW(ourAdapter->getConnection());
		REQUIRE(ourAdapter.get()!=nullptr);
		REQUIRE(ourAdapter->isConnected());
	}

	SECTION("Check our connection with bad arguments, but it should use default argument")
	{
		auto ourBadAdapter = PostgreAdapter::Instance();

		REQUIRE_NOTHROW(ourBadAdapter.get()->getConnection());
		REQUIRE(ourBadAdapter.get()->isConnected());
		REQUIRE(ourBadAdapter.get()!=nullptr);
	}

	SECTION("Check our query")
	{
		auto ourQuery = std::string_view{"SELECT * FROM users"};
		auto ourBadQuery = std::string_view{ "something * 123wrong" };

		REQUIRE_NOTHROW(ourAdapter.get()->query(ourQuery));
		REQUIRE_THROWS(ourAdapter->query(ourBadQuery));
		REQUIRE(ourAdapter->query(ourQuery)== std::nullopt);
	}

	SECTION("Check how properly we close connection")
	{
		REQUIRE_NOTHROW(ourAdapter.get()->closeConnection());
	}
}
