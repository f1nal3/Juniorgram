#include <catch2/catch.hpp>

#include "Models.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

using DataAccess::UserRegistration;
using DataAccess::User;
using DataAccess::UserInfo;

using DataAccess::PostgreAdapter;
using DataAccess::PGQueryBuilder;

TEST_CASE("Check user registration", "[dummy]")
{
    auto testTable = std::make_unique<PGQueryBuilder>("users", PostgreAdapter::Instance(DBOptions::test));

    SECTION("Wanna add a user", "Kinda a 'userRegistration method in repos'")
    {
        User testUser({"3", "aboba3@gmail.com", "aboba3", "kindahash3"});

        std::tuple insertData{std::pair{testUser.fieldName(UserInfo::ID), testUser[UserInfo::ID]},
                              std::pair{testUser.fieldName(UserInfo::EMAIL), testUser[UserInfo::EMAIL]}, 
                              std::pair{testUser.fieldName(UserInfo::LOGIN), testUser[UserInfo::LOGIN]}, 
                              std::pair{testUser.fieldName(UserInfo::PASSHASH), testUser[UserInfo::PASSHASH]}};

        auto responce = testTable->Insert()->columns(insertData)->execute();

        REQUIRE_FALSE(responce.has_value());
    }

    SECTION("We want to fill the table once", "Check how properly map fills")
    {
        User testUser;

        auto responce = testTable->Select()->columns({"*"})->Where("id = '1'")->execute();
        if (responce.has_value())
        {
            testUser.fillMap(*responce);

            REQUIRE(testUser[UserInfo::ID] == "1");
        }
    }
    SECTION("Deleting user", "Check agility of the model with our adapter")
    {

    }
}
