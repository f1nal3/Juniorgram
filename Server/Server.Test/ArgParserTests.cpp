#include <catch2/catch.hpp>
#include <ArgParser.hpp>

TEST_CASE("Constructor of ArgParser [ArgParser][Success]")
{
    SECTION("Default constructor")
    {
        std::vector<const char*> args = {"juniorgram"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
    }

    SECTION("Number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"juniorgram", "--any_flag"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::runtime_error);
    }

    SECTION("Number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"juniorgram", "-m", "999"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::runtime_error);
    }
	
    SECTION("Invalid arguments keys throw an exception")
    {
        std::vector<const char*> args = {"juniorgram", "-m", "--any_flag", "666666"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::runtime_error);
    }

    SECTION("Duplicated keys in arguments throw an exception")
    {
        std::vector<const char*> args = {"juniorgram", "--any_flag", "any_flag", "any_flag"};
        CHECK_THROWS(ArgParser(static_cast<int>(args.size()), args.data()));
    }
}

TEST_CASE("Method of obtaining a pair of arguments of the argparser [ArgParser][Success]")
{ 
    std::vector<const char*> args = {"juniorgram"};
    ArgParser                parser(static_cast<int>(args.size()), args.data());

    SECTION("Getting arguments with default pairs")
    {
        CHECK_NOTHROW(parser.getPair("--serverport"));
        CHECK_NOTHROW(parser.getPair("--dbname"));
        CHECK_NOTHROW(parser.getPair("--hostaddr"));
        CHECK_NOTHROW(parser.getPair("--port"));
        CHECK_NOTHROW(parser.getPair("--user"));
        CHECK_NOTHROW(parser.getPair("--password"));
    }
    
    SECTION("Check the server port flag pair with the default argument") 
    { 
        std::pair<std::string,std::string> serverPortPair{"--serverport", "65001"};
        REQUIRE(parser.getPair("--serverport") == serverPortPair);
    }

    SECTION("Check the database flag pair with the default argument")
    {
        std::pair<std::string,std::string> dbnamePair{"--dbname", "juniorgram"};
        REQUIRE(parser.getPair("--dbname") == dbnamePair);
    }

    SECTION("Check the host address flag pair with the default argument")
    {
        std::pair<std::string,std::string> hostaddrPair{"--hostaddr", "127.0.0.1"};
        REQUIRE(parser.getPair("--hostaddr") == hostaddrPair);
    }

    SECTION("Check the database port flag pair with the default argument")
    {
        std::pair<std::string, std::string> dbportPair{"--port", "5432"};
        REQUIRE(parser.getPair("--port") == dbportPair);
    }

    SECTION("Check the database user flag pair with the default argument")
    {
        std::pair<std::string, std::string> userPair{"--user", "postgres"};
        REQUIRE(parser.getPair("--user") == userPair);
    }

    SECTION("Check the database password flag pair with the default argument")
    {
        std::pair<std::string, std::string> passwordPair{"--password", "postgres"};
        REQUIRE(parser.getPair("--password") == passwordPair);
    }

    SECTION("Taking a bad flag") 
    { 
        CHECK_THROWS_AS(parser.getPair("--any_flag"), std::exception);
    }
}
