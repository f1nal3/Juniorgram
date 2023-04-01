#include <catch2/catch.hpp>
#include <ArgParser.hpp>

TEST_CASE("Constructor of ArgParser [ArgParser][Success]")
{
    SECTION("Default constructor")
    {
        std::vector<const char*> args = {"./juniorgram"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
    }

    SECTION("Constructor with default arguments")
    {
        std::vector<const char*> args = {"./juniorgram", "--serverport=65001", "--port=5432", 
            "--dbname=juniorgram", "--hostaddr=127.0.0.1", "--user=postgres", "--password=postgres"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("Number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"juniorgram", "-m"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::runtime_error);
    }

    SECTION("Calling the helper flag")
    {
        std::vector<const char*> args = {"./juniorgram", "--help"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
    }

    SECTION("Constructor with different arguments")
    {
        std::vector<const char*> args = {"./juniorgram", "--serverport=65003", "--port=6666", 
            "--dbname=testdb", "--hostaddr=255.255.0.0", "--user=tester", "--password=tester"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("deliberate disregard for the value of the argument of a server port")
    {
        std::vector<const char*> args = {"./juniorgram", "--serverport=6666666"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("deliberate disregard for the value of the argument of a database port")
    {   
        std::vector<const char*> args = {"./juniorgram", "--port=6666666"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("deliberate disregard for the value of the argument of a database")
    {
        {
            std::vector<const char*> args = {"./juniorgram", "--dbname=testdb"};
            CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
        }
        {
            std::vector<const char*> args = {"./juniorgram", "--dbname=otherdb"};
            CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
        }
    }

    SECTION("deliberate disregard for the value of the argument of a host address")
    {
        std::vector<const char*> args = {"./juniorgram", "--hostaddr=255.255.0.0"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }
	
    SECTION("deliberate disregard for the value of the argument of a database user")
    {   
        std::vector<const char*> args = {"./juniorgram", "--user=tester"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("deliberate disregard for the value of the argument of a database password")
    {
        std::vector<const char*> args = {"./juniorgram", "--password=tester"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()),args.data()));
    }

    SECTION("Invalid arguments keys throw an exception")
    {
        std::vector<const char*> args = {"./juniorgram", "-m", "any_flag=test", "666666"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::exception);
    }

    SECTION("Number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"./juniorgram", "-m", "999"};
        CHECK_THROWS_AS(ArgParser(static_cast<int>(args.size()), args.data()), std::runtime_error);
    }

    SECTION("Duplicated keys in arguments throw an exception")
    {
        std::vector<const char*> args = {"./juniorgram", "--serverport=65001", "--serverport=65001", "--serverport=65001"};
        CHECK_THROWS(ArgParser(static_cast<int>(args.size()), args.data()));
    }
}

TEST_CASE("Method of obtaining a pair of arguments of the argparser [ArgParser][Success]")
{ 
    std::vector<const char*> args = {"./juniorgram"};
    ArgParser                parser(static_cast<int>(args.size()), args.data());

    SECTION("Getting arguments with default pairs")
    {
        CHECK_NOTHROW(parser.getPair("serverport"));
        CHECK_NOTHROW(parser.getPair("dbname"));
        CHECK_NOTHROW(parser.getPair("hostaddr"));
        CHECK_NOTHROW(parser.getPair("port"));
        CHECK_NOTHROW(parser.getPair("user"));
        CHECK_NOTHROW(parser.getPair("password"));
    }
    
    SECTION("Check the server port flag pair with the default argument") 
    { 
        std::pair<std::string,std::string> serverPortPair{"--serverport", "65001"};
        REQUIRE(parser.getPair("--serverport") == serverPortPair);
    }

    SECTION("Check the any flag pair with the other argument")
    {
        std::vector<const char*> sectionArgs = {"./juniorgram", "--serverport=65003"};
        ArgParser                sectionParser(static_cast<int>(sectionArgs.size()), sectionArgs.data());

        std::pair<std::string, std::string> serverPortPair{"--serverport", "65003"};
        REQUIRE(sectionParser.getPair("--serverport").second == serverPortPair.second);
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
        CHECK_THROWS_AS(parser.getPair("any_flag"), std::exception);
    }
}
