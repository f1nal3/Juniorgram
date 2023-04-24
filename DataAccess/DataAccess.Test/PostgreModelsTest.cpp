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

TEST_CASE("Check user registration", "Filling the table")
{
    SECTION("We want to fill the table once")
    {
        User testUser;
        auto testTable = std::make_unique<PGQueryBuilder>("users", PostgreAdapter::Instance(DBOptions::test));

        auto responce = testTable->Select()->columns({"*"})->Where("id = '1'")->execute();
        if (responce.has_value())
        {
            testUser.fillMap(*responce);

            REQUIRE(testUser[UserInfo::ID] == "1");
        }
    }
}
