#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <iostream>

#include <DataAccess.Postgre/PostgreTable.hpp>
#include <DataAccess.Postgre/DataBaseOptions.hpp>

DataAccess::PostgreTable test("testing", DBOptions::test);

TEST_CASE("Query", "[PostgreAdapter]")
{
    //SECTION("Delete test") 
    //{
    //   REQUIRE_NOTHROW(DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>(
    //                   "hostaddr=104.40.239.183 port=5432 dbname=test user=userfortests password=123")
    //                    ->query("delete from test_table"));
    //}
    //SECTION("Insert test") 
    //{ 
    //    REQUIRE_NOTHROW(DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>(
    //                    "hostaddr=104.40.239.183 port=5432 dbname=test user=userfortests password=123")
    //                    ->query("insert into test_table values(0, 5, 'male')"));
    //}
    //SECTION("Select test")
    //{
    //    auto result =   DataAccess::PostgreAdapter::getInstance<DataAccess::PostgreAdapter>(
    //                    "hostaddr=104.40.239.183 port=5432 dbname=test user=userfortests password=123")
    //                    ->query("select * from test_table;").value();
    //    
    //   REQUIRE(std::any_cast<pqxx::result>(result).empty() == false);
    //}
}

TEST_CASE("Select[Where]", "[PostgreAbstractionLayout]")
{ 
	std::string query = test.Select()->columns({"*"})->Where("id > 5")->getStringQuery();

	SECTION("Query string test") 
	{
        std::cout << "[" << query << "]\n";
		REQUIRE(query == "select * from testing where id > 5");
        test.Select()->rollback();
	};
}

TEST_CASE("Select[Where not]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->Where("not name = 'Max'")->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where not name = 'Max'");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[Where and]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->columns({"*"})->Where("name = 'Max' and id > 10")->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where name = 'Max' and id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[Where or]", "[PostgreAbstractionLayout]")
{
    std::string query =
        test.Select()->columns({"*"})->Where("name = 'Max' or id > 10")->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing where name = 'Max' or id > 10");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[limit]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->limit(1)->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "select * from testing limit 1 offset 0");
        test.Select()->rollback();
    };
}

TEST_CASE("Select[orderBy]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Select()->columns({"*"})->orderBy({"name"})->getStringQuery();

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
                             Where("not name = 'A' and (id < 0 or id > 15)")->
                             orderBy({"name", "id"})->
                             limit(5, 25)->
                             getStringQuery();

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
                            ->Where("id not")
                            ->Between(0, 15)
                            ->And("name <> 'A'")
                            ->getStringQuery();
         
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
    std::string query = test.Insert()->field(1, 1.5, "a")->getStringQuery();

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
    std::string query = test.Insert()->field(testTuple)->getStringQuery();

    SECTION("Query string test") 
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[fields][3]", "[PostgreAbstractionLayout]")
{
     std::string query = test.Insert()->columns(std::pair{"A", 1}, std::pair{"B", 1.5}, std::pair{"C", "a"})->getStringQuery();

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
    std::string query = test.Insert()->columns(testTuple)->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "insert into testing(A, B, C) values(1, 1.5, 'a')");
        test.Insert()->rollback();
    }
}

TEST_CASE("Insert[returning]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Insert()->field(1, 1.5, "a")->field(1, 1.5, "a")->returning({"*"})->getStringQuery();

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
        test.Update()->fields(std::pair{"A", 1}, std::pair{"B", 1.5}, std::pair{"C", "a"})->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "update testing set A = 1, B = 1.5, C = 'a'");
        test.Update()->rollback();
    }
}

TEST_CASE("Update[fields Where]", "[PostgreAbstractionLayout]")
{
    std::tuple testTuple{
        std::pair{"A", 1},
        std::pair{"B", 1.5},
        std::pair{"C", "a"}
    };
    std::string query = test.Update()->fields(testTuple)->Where("C <> 'a'")->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "update testing set A = 1, B = 1.5, C = 'a' where C <> 'a'");
        test.Update()->rollback();
    }
}

TEST_CASE("Delete[Where]", "[PostgreAbstractionLayout]")
{
    std::string query = test.Delete()->Where("A = 1")->getStringQuery();

    SECTION("Query string test")
    {
        std::cout << "[" << query << "]\n";
        REQUIRE(query == "delete from testing where A = 1");
        test.Delete()->rollback();
    }
}
