#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

#include <Models/Models.hpp>
#include <Models/UnifiedModel.hpp>

#include "PGModelFiller.hpp"

using Models::New::UserInfo;
using Models::New::ChannelData;
using Models::New::User;
using Models::New::Channel;
using DataAccess::PGModelFiller;
using DataAccess::PostgreAdapter;
using DataAccess::PGQueryBuilder;


TEST_CASE("Check model usage", "[dummy]")
{
    auto testTable = std::make_unique<PGQueryBuilder>("users", PostgreAdapter::Instance(DBOptions::test));

    PGModelFiller testFiller;
    
    SECTION("Wanna add a user", "Kinda a 'userRegistration method in repos'")
    {
        
        User testUser({ {UserInfo::ID,"3"},
                                    {UserInfo::EMAIL,"aboba3@gmail.com"},
                                    {UserInfo::LOGIN,"aboba3"},
                                    {UserInfo::PASSHASH,"kindahash3"} });

        auto responce = testTable->Insert()->columns(&testUser)->execute();

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
                testFiller.fill(responce->begin(), &testUser);
                

                REQUIRE(testUser[UserInfo::ID] == "1");
            }
        }

        SECTION("We define fields")
        {
            User testUser;

            auto responce = testTable->Select()->
                columns({ testUser.resolveName(UserInfo::EMAIL)
                        , testUser.resolveName(UserInfo::LOGIN) })->
                Where(testUser.resolveName(UserInfo::ID) + " = '3'")->execute();
           
            if (responce.has_value())
            {
                REQUIRE_NOTHROW(testFiller.fill(responce->begin(), &testUser));

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
            {ChannelData::CHANNEL_NAME, testChannelName},
            {ChannelData::CHANNEL_USER_LIMIT, "1000"},
            {ChannelData::CREATOR_ID, "3"} });

        SECTION("Model constructor test")
        {
            REQUIRE(testChannel[ChannelData::CHANNEL_NAME] == testChannelName);
        }
        
        SECTION("Adding channel in DB")
        {          
            auto result = testTable->Insert()->columns(&testChannel)->execute();

            REQUIRE_FALSE(result.has_value());
        }

        SECTION("An attempt to imitate createChannel method")
        {
            DataAccess::ChannelsRepository testChannelRepos(DataAccess::PostgreAdapter::Instance());

            SECTION("Bad attempt")
            {
                Channel testBadChannel({
                    { ChannelData::CREATOR_ID, "3"},
                    { ChannelData::CHANNEL_NAME, testChannel[ChannelData::CHANNEL_NAME]},
                    { ChannelData::CHANNEL_USER_LIMIT, "10000"} });

                REQUIRE(testChannelRepos.newCreateChannel(testBadChannel) == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED);
            }

            SECTION("Good attempt", "Via new method")
            {                
                Channel testNewChannel({
                    { ChannelData::CREATOR_ID, "3"},
                    { ChannelData::CHANNEL_NAME, "newTestChannel" },
                    { ChannelData::CHANNEL_USER_LIMIT, "10000"} });
               
                REQUIRE(testChannelRepos.newCreateChannel(testNewChannel) == Utility::ChannelCreateCodes::SUCCESS);
            }
        }
    }
};
