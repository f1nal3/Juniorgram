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

	auto testAdapter = PostgreAdapter::Instance(DBOptions::test);

	SECTION("Check our connection")
	{
		REQUIRE_NOTHROW(testAdapter->getConnection());
		REQUIRE(testAdapter.get()!=nullptr);
		REQUIRE(testAdapter->isConnected());
	}

	SECTION("Check our connection with bad arguments, but it should use default argument")
	{
		auto testBadAdapter = PostgreAdapter::Instance();

		REQUIRE_NOTHROW(testBadAdapter.get()->getConnection());
		REQUIRE(testBadAdapter.get()->isConnected());
		REQUIRE(testBadAdapter.get()!=nullptr);
	}

	SECTION("Check our query")
	{
		auto testQuery = std::string_view{"SELECT * FROM users"};
		auto testBadQuery = std::string_view{ "something * 123wrong" };

		REQUIRE_NOTHROW(testAdapter.get()->query(testQuery));
		REQUIRE_THROWS(testAdapter->query(testBadQuery));
		REQUIRE(testAdapter->query(testQuery)== std::nullopt);
	}

	SECTION("Check how properly we close connection")
	{
		REQUIRE_NOTHROW(testAdapter.get()->closeConnection());
	}
}
