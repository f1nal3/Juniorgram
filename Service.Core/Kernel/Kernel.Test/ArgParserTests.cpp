#include <catch2/catch.hpp>

#include <ArgParser.hpp>

TEST_CASE("Constructor of ArgParser", "[ArgParser][Success]")
{
    SECTION("Default constructor")
    {
        std::vector<const char*> args {"./juniorgram"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(args.size()), args.data()));
    }

    SECTION("Constructor with default arguments and default initialization")
    {
       std::vector<const char*> defaultArgs {"./juniorgram", 
                                              "--serviceport=65001", 
                                              "--dbport=5432", "--dbname=juniorgram",
                                              "--hostaddr=127.0.0.1", 
                                              "--user=postgres", "--password=postgres"};
       CHECK_NOTHROW(ArgParser(static_cast<int>(defaultArgs.size()), defaultArgs.data()));
    }

    SECTION("Constructor with other initialization")
    {
        std::vector<const char*> otherDefaultArgs{"./juniorgram",
                                             "--serviceport", "65003",
                                             "--dbport", "6432",
                                             "--dbname", "testdb",
                                             "--hostaddr", "0.0.0.0",
                                             "--user", "user", 
                                             "--password", "user"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(otherDefaultArgs.size()), otherDefaultArgs.data()));
    }

    SECTION("Deliberate disregard for the value of the argument")
    {
        std::vector<const char*> limitedArgs {"./juniorgram", 
                                              "--serviceport=6666666", 
                                              "--dbport=6666666", "--dbname=otherdb",
                                              "--hostaddr=126.0.0.1", 
                                              "--user=tester", "--password=tester"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(limitedArgs.size()), limitedArgs.data()));
    }

    SECTION("Calling the helper flag")
    {
        std::vector<const char*> helperArgs {"./juniorgram", "--help"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(helperArgs.size()), helperArgs.data()));
    }

    SECTION("Calling the abridged version of helper flag")
    {
        std::vector<const char*> helperArgs{"./juniorgram", "-h"};
        CHECK_NOTHROW(ArgParser(static_cast<int>(helperArgs.size()), helperArgs.data()));
    }
}

TEST_CASE("Constructor of ArgParser", "[ArgParser][Failed]")
{
     SECTION("Duplicated keys in arguments throw an exception")
     {
         std::vector<const char*> failedArgs {"./juniorgram", "--serviceport=65001", "--serviceport=65001"};
         CHECK_THROWS(ArgParser(static_cast<int>(failedArgs.size()), failedArgs.data()));
     }

     SECTION("Invalid arguments keys throw an exception")
     {
         std::vector<const char*> failedArgs {"./juniorgram", "any_flag", "test_flag"};
         CHECK_THROWS_AS(ArgParser(static_cast<int>(failedArgs.size()), failedArgs.data()), std::exception);
     }

     SECTION("Number of keys without values throws an exception")
     {
         std::vector<const char*> failedArgs {"./juniorgram", "-m", "999"};
         CHECK_THROWS_AS(ArgParser(static_cast<int>(failedArgs.size()), failedArgs.data()), std::exception);
     }

     SECTION("Parsing incorrectly filled arguments")
     {
         std::vector<const char*> failedArgs {"./juniorgram",
                                              "serviceport=test_key", "dbport=test_key",
                                              "dbname=test_key",     "hostaddr=test_key",
                                              "user=test_key",       "password=test_key"};
         CHECK_THROWS(ArgParser(static_cast<int>(failedArgs.size()), failedArgs.data()));
     }
}

TEST_CASE("Method of obtaining a pair of arguments of the argparser", "[ArgParser][Success]")
{ 
    std::vector<const char*> args {"./juniorgram"};
    ArgParser                parser(static_cast<int>(args.size()), args.data());

    SECTION("Getting arguments with default pairs")
    {
        CHECK_NOTHROW(parser.getPair("serviceport"));
        CHECK_NOTHROW(parser.getPair("dbname"));
        CHECK_NOTHROW(parser.getPair("hostaddr"));
        CHECK_NOTHROW(parser.getPair("dbport"));
        CHECK_NOTHROW(parser.getPair("user"));
        CHECK_NOTHROW(parser.getPair("password"));
    }
    
    SECTION("Check the service port flag pair with the default argument") 
    { 
        std::pair<std::string,std::string> servicePortPair{"--serviceport", "65001"};
        REQUIRE(parser.getPair("--serviceport") == servicePortPair);
    }

    SECTION("Check the any flag pair with the other argument")
    {
        std::vector<const char*> sectionArgs = {"./juniorgram", "--serviceport=65003"};
        ArgParser                sectionParser(static_cast<int>(sectionArgs.size()), sectionArgs.data());
        REQUIRE_NOTHROW(sectionParser);

        auto otherPair = std::make_pair("--serviceport", "65003");
        REQUIRE(sectionParser.getPair("--serviceport").second == otherPair.second);
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
        std::pair<std::string, std::string> dbportPair{"--dbport", "5432"};
        REQUIRE(parser.getPair("--dbport") == dbportPair);
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
