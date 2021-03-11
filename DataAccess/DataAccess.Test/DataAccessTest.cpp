#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "DataAccess.Static/DataBasePostgreSQL.hpp"
#include "DataAccess.Static/DataBasePostgreSQLOperations.hpp"


TEST_CASE("Tested the main object", "[PostgreSQL]")
{
    auto psql = DBPostgre::PostgreSQL::getPostgre(
        "dbname=postgres user=postgres password= hostaddr=127.0.0.1 port=5432");

    REQUIRE(psql->isConnected() == true);
    REQUIRE(psql->query("select 1;").value().at(0).at(0).as<int>() == 1);
}

TEST_CASE("Tested postgre tables operations", "[PostgreTableOperations]")
{
    using namespace std::literals::string_view_literals;

    auto psql = DBPostgre::PostgreSQL::getPostgre();
    auto tblo = DBPostgre::PostgreTableOperations(psql);

    SECTION("creating the table")
    {
        tblo.createTable("TestTable"sv, "id integer"sv);
        REQUIRE(psql->query(
            "select * from pg_tables where tablename = 'TestTable';"sv
            ).has_value() == true);
    }
   
    SECTION("droping the table")
    {
        tblo.deleteTable("TestTable"sv);
        REQUIRE(psql->query(
            "select * from pg_tables where tablename = 'TestTable';"sv
            ).has_value() == false);
    }
}

TEST_CASE("Tested postgre fields operations", "[PostgreFieldOperations]")
{
    using namespace std::literals::string_view_literals;

    auto psql = DBPostgre::PostgreSQL::getPostgre();
    auto fldo = DBPostgre::PostgreFieldOperations(psql);


    SECTION("is data exist")
    {
        REQUIRE(fldo.isExist("TestFields"sv, "id"sv, "0") == true);
    }
    SECTION("inserting the data")
    {
        fldo.insert("TestFields"sv, "1", "id"sv);
        REQUIRE(fldo.isExist("TestFields"sv, "id"sv, "1") == true);
    }
    SECTION("selecting the data")
    {
        REQUIRE(fldo.select("TestFields", "*").empty() == false);
    }
    SECTION("updating the data")
    {
        fldo.update("TestFields"sv, "id = 10"sv, "where id = 0"sv);
        REQUIRE(fldo.isExist("TestFields"sv, "id"sv, "10") == true);
    }
    SECTION("deleting the data")
    {
        fldo.del("TestFields"sv, "where id = 10"sv);
        REQUIRE(fldo.isExist("TestFields"sv, "id"sv, "10") == false);
    }
}

TEST_CASE("Tested columns postgre operations", "[PostgreColumnOperations]")
{
    using namespace std::literals::string_view_literals;

    auto psql = DBPostgre::PostgreSQL::getPostgre();
    auto fldo = DBPostgre::PostgreFieldOperations(psql);
    auto clnp = DBPostgre::PostgreColumnOperations(psql);

    SECTION("adding new column")
    {
        clnp.addNewColumn("TestColumn"sv, "newId"sv, "integer"sv);
        fldo.insert("TestColumn"sv, "0, 0", "id, newId"sv);
        REQUIRE(fldo.isExist("TestColumn"sv, "newId"sv, "0") == true);
    }
    SECTION("deleting the column")
    {
        clnp.deleteColumn("TestColumn"sv, "newId");
        REQUIRE(fldo.isExist("TestColumn"sv, "newId"sv, "0") == false);
    }
}