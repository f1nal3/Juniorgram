#include <catch2/catch.hpp>

#include "Models.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

using DataAccess::UserRegistration;
using DataAccess::User;
using DataAccess::UserInfo;

using DataAccess::Channel;
using DataAccess::ChannelInfo;

using DataAccess::PostgreAdapter;
using DataAccess::PGQueryBuilder;

TEST_CASE("Check model usage", "[dummy]")
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
        SECTION("All fields at once")
        {
            User testUser;

            auto responce = testTable->Select()->columns({ "*" })->Where("id = '1'")->execute();

            if (responce.has_value())
            {
                testUser.fillMap(responce->begin());

                REQUIRE(testUser[UserInfo::ID] == "1");
            }
        }
        SECTION("We define fields")
        {
            User testUser;

            auto responce = testTable->Select()->
                columns({ testUser.fieldName(UserInfo::EMAIL)
                        , testUser.fieldName(UserInfo::LOGIN) })->
                Where(testUser.fieldName(UserInfo::ID) + " = '3'")->execute();
           
            if (responce.has_value())
            {
                REQUIRE_NOTHROW(testUser.fillMap(responce->begin()));

                REQUIRE(testUser[UserInfo::LOGIN] == "aboba3");
                REQUIRE(testUser[UserInfo::ID] == "");
            }
        }
    }
};

TEST_CASE("ChannelModel", "Check how easily we can use this model in PGRepos")
{
    auto testTable = std::make_unique<PGQueryBuilder>("channels", PostgreAdapter::Instance(DBOptions::test));

    SECTION("Creating channel")
    {
        std::string testChannelName = { "myFirstChannel" };

        Channel testChannel({
            {ChannelInfo::CHANNEL_NAME, testChannelName},
            {ChannelInfo::CHANNEL_USER_LIMIT, "1000"},
            {ChannelInfo::CREATOR_ID, "3"} });

        std::tuple insertData = {
                std::pair{testChannel.fieldName(ChannelInfo::CHANNEL_NAME),testChannel[ChannelInfo::CHANNEL_NAME]},
                std::pair{testChannel.fieldName(ChannelInfo::CREATOR_ID), testChannel[ChannelInfo::CREATOR_ID]},
                std::pair{testChannel.fieldName(ChannelInfo::CHANNEL_USER_LIMIT),testChannel[ChannelInfo::CHANNEL_USER_LIMIT]} };

        SECTION("Model constructor test")
        {
            REQUIRE(testChannel[ChannelInfo::CHANNEL_NAME] == testChannelName);
        }
        
        SECTION("Adding channel in DB")
        {          
            auto result = testTable->Insert()->columns(insertData)->execute();

            REQUIRE_FALSE(result.has_value());
        }

        SECTION("An attempt to imitate createChannel method")
        {
            SECTION("Bad attempt")
            {
                auto findIdChannel = testTable->Select()->
                    columns({ testChannel.fieldName(ChannelInfo::CHANNEL_NAME) })->
                    Where(testChannel.fieldName(ChannelInfo::CHANNEL_NAME) + " = '" + testChannel[ChannelInfo::CHANNEL_NAME] + "'")->
                    execute();

                REQUIRE(findIdChannel.has_value()); // Expands to end of function due to existing of channel
            }
        }
    };

};
