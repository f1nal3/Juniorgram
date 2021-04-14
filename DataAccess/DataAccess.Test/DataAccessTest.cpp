#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>

#include "DataAccess.Static/PostgreAbstractionLayout.hpp"

DataAccess::Table test("testing");

TEST_CASE("Select[where]", "[PostgreAbstractionLayout]")
{ 
	std::string query = test.Select()->fields({"*"})->where("id > 5")->getQuery();

	SECTION("Query string test") 
	{
		std::cout << "[" << query << "]" << '\n';
		REQUIRE(query == "select * from testing where id > 5");
        test.Select()->rollback();
	};
}

TEST_CASE("Select[where not]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->fields({"*"})->where()->Not("name = 'Max'")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select * from testing where not name = 'Max'");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[where and]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->fields({"*"})->where("name = 'Max'")->And("id > 10")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select * from testing where name = 'Max' and id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[where or]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->fields({"*"})->where("name = 'Max'")->Or("id > 10")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select * from testing where name = 'Max' or id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[limit]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->fields({"*"})->limit(1)->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select * from testing limit 1 offset 0");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[orderBy]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->fields({"*"})->orderBy({"name"})->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select * from testing order by name");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[Extra]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->
                             fields({"id", "name"})->
                             where()->Not("name = 'A'")->
                             And("(id < 0")->
                             Or("id > 15)")->
                             orderBy({"name", "id"})->
                             limit(5, 25)->
                             getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]" << '\n';
        REQUIRE(query == "select id, name from testing where not name = 'A' and (id < 0 or id > 15) order by name, id limit 5 offset 25");
        test.Select()->rollback();
    };
}
