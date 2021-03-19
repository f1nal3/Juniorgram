#include <catch2/catch.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <ArgumentParser.hpp>

TEST_CASE("Constructor argument parser")
{
    SECTION("Check a job with KeysValidator")
    {
        KeysValidator validator({"-r", "-k"}, {"-k"});
        std::vector<const char*> args = {"path_to_project", "-k", "-r", "55000", "-d"};
        CHECK_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data(), validator));
    }

    SECTION("Only 1 input parameter throws an exception")
    {
        std::vector<const char*> args = {"path_to_project"};
        CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()), std::exception);
    }

    SECTION("Even number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"path_to_project", "-p"};
        CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()), std::exception);
    }

    SECTION("Odd number of keys without values throws an exception")
    {
        std::vector<const char*> args = {"path_to_project", "-d", "131"};
        CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()), std::exception);
    }

    SECTION("Unvalid arguments keys throw an exeption")
    {
        {
            std::vector<const char*> args = {"path_to_project", "any_key", "641", "-d"};
            CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()),
                            std::exception);
        }

        {
            std::vector<const char*> args = {"path_to_project", "-p", "27015", "any_key", "650000"};
            CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()),
                            std::exception);
        }
    }

    SECTION("Duplicated keys in arguments throw an exception")
    {
        {
            std::vector<const char*> args = {"path_to_project", "-p", "27015", "-p", "6000"};
            CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()),
                            std::exception);
        }

        {
            std::vector<const char*> args = {"path_to_project", "-d", "-d", "-d"};
            CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()),
                            std::exception);
        }
    }

    SECTION("Coexistence realDB and fileDB keys in arguments throws an exception")
    {
        std::vector<const char*> args = {"path_to_project", "-p",
                                         "27015"
                                         "-d"};
        CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data()), std::exception);
    }

    SECTION("Not existing realDB and fileDB key in arguments throws an exception")
    {
        std::vector<const char*> args = {"path_to_project", "-k", "-r"};
        CHECK_THROWS_AS(ArgumentParser(static_cast<int>(args.size()), args.data(),
                                       KeysValidator({"-k", "-r"}, {})),
                        std::exception);
    }
}

TEST_CASE("getPort()")
{
    SECTION("realDB key")
    {
        SECTION("Too big port value throws an exception")
        {
            std::vector<const char*> args = {"path_to_project", "-p", "99000"};
            ArgumentParser ap(static_cast<int>(args.size()), args.data());
            CHECK_THROWS_AS(ap.getPort(), std::exception);
        }

        SECTION("Too small port value throws an exception")
        {
            std::vector<const char*> args = {"path_to_project", "-p", "-2"};
            ArgumentParser ap(static_cast<int>(args.size()), args.data());
            CHECK_THROWS_AS(ap.getPort(), std::exception);
        }

        SECTION("getPort() == 27015")
        {
            {
                std::vector<const char*> args = {"path_to_project", "-p", "27015"};
                REQUIRE_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data()));
                ArgumentParser ap(static_cast<int>(args.size()), args.data());
                REQUIRE(ap.getPort() == 27015);
            }

            {
                std::vector<const char*> args = {"path_to_project", "   -p    ", "   27015   "};
                REQUIRE_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data()));
                ArgumentParser ap(static_cast<int>(args.size()), args.data());
                REQUIRE(ap.getPort() == 27015);
            }
        }
    }

    SECTION("fileDB key")
    {
        SECTION("getPort() throws an exception")
        {
            std::vector<const char*> args = {"path_to_project", "-d"};
            REQUIRE_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data()));
            ArgumentParser ap(static_cast<int>(args.size()), args.data());
            REQUIRE_THROWS_AS(ap.getPort(), std::exception);
        }
    }
}

TEST_CASE("isRealDB()")
{
    SECTION("with realDB key isRealDB() == true")
    {
        std::vector<const char*> args = {"path_to_project", "-p", "27015"};
        REQUIRE_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data()));
        ArgumentParser ap(static_cast<int>(args.size()), args.data());
        REQUIRE(ap.isRealDB() == true);
    }

    SECTION("with fileDB key isRealDB() == false")
    {
        std::vector<const char*> args = {"path_to_project", "-d"};
        REQUIRE_NOTHROW(ArgumentParser(static_cast<int>(args.size()), args.data()));
        ArgumentParser ap(static_cast<int>(args.size()), args.data());
        REQUIRE(ap.isRealDB() == false);
    }
}