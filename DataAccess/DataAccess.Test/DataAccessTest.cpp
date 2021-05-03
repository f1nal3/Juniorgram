#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>

#include "DataAccess.Static/PostgreAbstractionLayout.hpp"

DataAccess::Table test("testing");

TEST_CASE("Select[where]", "[PostgreAbstractionLayout]")
{ 
	std::string query = test.Select()->columns({"*"})->where("id > 5")->getQuery();

	SECTION("Query string test") 
	{
        std::cout << "[" << query << "]\n";
		REQUIRE(query == "select * from testing where id > 5");
        test.Select()->rollback();
	};
}

TEST_CASE("Select[where not]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->where()->Not("name = 'Max'")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where not name = 'Max'");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[where and]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->columns({"*"})->where("name = 'Max'")->And("id > 10")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where name = 'Max' and id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[where or]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->columns({"*"})->where("name = 'Max'")->Or("id > 10")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where name = 'Max' or id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[limit]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->limit(1)->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing limit 1 offset 0");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[orderBy]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->orderBy({"name"})->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing order by name");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[Extra][1]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->
                             columns({"id", "name"})->
                             where()->Not("name = 'A'")->
                             And("(id < 0")->
                             Or("id > 15)")->
                             orderBy({"name", "id"})->
                             limit(5, 25)->
                             getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select id, name from testing where not name = 'A' and (id < 0 or id > 15) order by name, id limit 5 offset 25");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[Extra][2]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()
                            ->distinct()
                            ->columns({"id, name"})
                            ->where("id")
                            ->Not()
                            ->between(0, 15)
                            ->And("name <> 'A'")
                            ->getQuery();
         
    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query ==
                "select distinct id, name from testing where id not between 0 and 15 and name <> 'A'");
        test.Select()->rollback();
    };
}


TEST_CASE("Insert[fields][1]", "[PostgreAbstractionLayout]")
{ 
    std::string query = test.Insert()->field(1, 1.5, "a")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[fields][2]", "[PostgreAbstractionLayout]")
{
    std::tuple testTuple{1, 1.5, "a"};
    std::string query = test.Insert()->field(testTuple)->getQuery();

    SECTION("Query string test") 
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[fields][3]", "[PostgreAbstractionLayout]")
{
     std::string query = test.Insert()->columns(std::pair{"A", 1}, std::pair{"B", 1.5}, std::pair{"C", "a"})->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing(A, B, C) values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[fields][4]", "[PostgreAbstractionLayout]")
{
    std::tuple testTuple
    {
        std::pair{"A", 1},
        std::pair{"B", 1.5},
        std::pair{"C", "a"}
    };
    std::string query = test.Insert()->columns(testTuple)->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing(A, B, C) values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[returning]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Insert()->field(1, 1.5, "a")->field(1, 1.5, "a")->returning({"*"})->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing values(1, 1.5, 'a'), (1, 1.5, 'a') returning *");
        test.Insert()->rollback();
    }
}

TEST_CASE("Update[fields]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Update()->fields(std::pair{"A", 1}, std::pair{"B", 1.5}, std::pair{"C", "a"})->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "update testing set A = 1, B = 1.5, C = 'a'");
        test.Update()->rollback();
    }
}

TEST_CASE("Update[fields where]", "[PostgreAbstractionLayout]")
{
    std::tuple testTuple{
        std::pair{"A", 1},
        std::pair{"B", 1.5},
        std::pair{"C", "a"}
    };
    std::string query = test.Update()->fields(testTuple)->where("C <> 'a'")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "update testing set A = 1, B = 1.5, C = 'a' where C <> 'a'");
        test.Update()->rollback();
    }
}

TEST_CASE("Delete[where]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Delete()->where("A = 1")->getQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "delete from testing where A = 1");
        test.Delete()->rollback();
    }
}
