#include <catch2/catch.hpp>

#include "DataAccess.Postgre/DataBaseOptions.hpp"
#include "DataAccess.Postgre/PostgreAdapter.hpp"
#include "DataAccess.Postgre/PostgreRepositories.hpp"

using DataAccess::RegisterRepository;
using DataAccess::ChannelsRepository;
using DataAccess::DirectMessageRepository;
using DataAccess::LoginRepository;
using DataAccess::MessagesRepository;
using DataAccess::PostgreAdapter;
using DataAccess::PostgreTable;

TEST_CASE("PostgreRepositories test", "[dummy]")
{
	auto testEmail{ "anotheruser@gmail.com" };
	auto testLogin{ "anotheruser" };
	auto testPassHash{ "65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5" };

	auto testTable = std::make_unique<PostgreTable>("users", PostgreAdapter::Instance());
	auto testInstance = PostgreAdapter::Instance();

	SECTION("Register repository")
	{
		SECTION("Register repos constructor")
		{
			REQUIRE_NOTHROW(RegisterRepository(PostgreAdapter::Instance()));
		}

		SECTION("Register repos constructor but reference already used")
		{
			REQUIRE_NOTHROW(RegisterRepository(testInstance));
		}

		RegisterRepository testRegisterRepos(PostgreAdapter::Instance());

		SECTION("Register user")
		{
			Network::RegistrationInfo ourUser(testEmail, testLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(ourUser) == Utility::RegistrationCodes::SUCCESS);
		}
		SECTION("Trying to register new user with data of already existing user")
		{
			auto newTestLogin{ "newuser" };
			Network::RegistrationInfo badUserEmail(testEmail, newTestLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(badUserEmail) == Utility::RegistrationCodes::EMAIL_ALREADY_EXISTS);

			auto newTestEmail{ "newuser@gmail.com" };
			Network::RegistrationInfo badUserLogin(newTestEmail, testLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(badUserLogin) == Utility::RegistrationCodes::LOGIN_ALREADY_EXISTS);
		}
	}

	SECTION("Login repository")
	{
		SECTION("Login repos constructor")
		{
			REQUIRE_NOTHROW(LoginRepository(PostgreAdapter::Instance()));
		}

		LoginRepository testLoginRepos(PostgreAdapter::Instance());
		auto testUserID = testTable->Select()->columns({ "id" })->Where("login='" + std::string(testLogin) + "'")->execute().value();

		SECTION("Logging")
		{
			Network::LoginInfo testUser(testLogin, testPassHash);
			REQUIRE(testLoginRepos.loginUser(testUser) == testUserID[0][0].as<uint64_t>());

			REQUIRE_NOTHROW(testLoginRepos.loginUser(testUser));
		}
		SECTION("Let's try to login with invalid values")
		{
			auto testBadHash{ "asdt24rersdf*_fs9dfs" };
			Network::LoginInfo testBadUser(testLogin, testBadHash);
			REQUIRE(testLoginRepos.loginUser(testBadUser) == 0);

			REQUIRE_NOTHROW(testLoginRepos.loginUser(testBadUser));
		}
	}

	SECTION("Channels repository")
	{
		auto testUserID = testTable->Select()->columns({ "id" })
			->Where("login = '" + std::string(testLogin) + "'")
			->execute().value()[0][0].as<uint64_t>();

		ChannelsRepository testChannelRepos(PostgreAdapter::Instance());
		auto testChannelName{ "testChannel" };
		///if we have no one channel in DB -> return 1 as ID of channel, else -> return lastID
		auto testChannelID = testChannelRepos.getAllChannelsList().empty() ? 1 : (testChannelRepos.getAllChannelsList().back().channelID );

		SECTION("Channels repos constructor")
		{
			REQUIRE_NOTHROW(ChannelsRepository(PostgreAdapter::Instance()));
		}

		SECTION("getAllChannels but no one exists")
		{
			REQUIRE_NOTHROW(testChannelRepos.getAllChannelsList());
			REQUIRE(testChannelRepos.getAllChannelsList().empty());
		}

		SECTION("Let's get empty channels subscription list")
		{
			REQUIRE_THROWS(testChannelRepos.getChannelSubscriptionList(testUserID).empty());
		}

		SECTION("Create channel")
		{
			Network::ChannelInfo testChannel(testUserID, testChannelID, testChannelName);

			SECTION("Create new channel with valid data")
			{
				REQUIRE(testChannelRepos.createChannel(testChannel) == Utility::ChannelCreateCodes::SUCCESS);
			}

			SECTION("Let's try to create channel with data of already existing channel")
			{
				REQUIRE(testChannelRepos.createChannel(testChannel) == Utility::ChannelCreateCodes::CHANNEL_ALREADY_CREATED);
			}

			SECTION("Try to make an error")
			{
				auto testChannelBadID{ 0 };
				auto testChannelBadName{ "" };
				Network::ChannelInfo testBadChannel(0, testChannelBadID, testChannelBadName);
				REQUIRE_THROWS(testChannelRepos.createChannel(testBadChannel));
			}
		}

		SECTION("Get all channels but at least one must exist")
		{
			REQUIRE(!testChannelRepos.getAllChannelsList().empty());
		}

		SECTION("Let's get channels subscription list")
		{
			REQUIRE(!testChannelRepos.getChannelSubscriptionList(testUserID).empty());
		}

		SECTION("Subcribe to channel")
		{
			Network::ChannelSubscriptionInfo testSubsInfo(testChannelID);
			testSubsInfo.userID = testUserID;

			SECTION("What if our user has already signed to channel")
			{
				REQUIRE(testChannelRepos.subscribeToChannel(testSubsInfo) == Utility::ChannelSubscribingCodes::CHANNEL_HAS_ALREADY_BEEN_SIGNED);
			}

			SECTION("Create new user and sign it to our channel")
			{
				RegisterRepository testRegNewUser(PostgreAdapter::Instance());

				auto testNewUserHash{ "asdadagfdgdfgdfgdfg" };
				auto testNewUserLogin{"seconduser"};
				auto testNewUserEmail{ "seconduser@gmail.com" };

				Network::RegistrationInfo testRegNewInfo(testNewUserEmail, testNewUserLogin, testNewUserHash);
				testRegNewUser.registerUser(testRegNewInfo);
				testSubsInfo.userID = testTable->Select()->columns({ "id" })->Where("login ='" + std::string(testNewUserLogin) + "'")->execute().value()[0][0].as<uint16_t>();

				REQUIRE(testChannelRepos.subscribeToChannel(testSubsInfo) == Utility::ChannelSubscribingCodes::SUCCESS);

				Network::ChannelLeaveInfo testNewLeaveInfo(testSubsInfo.userID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testNewLeaveInfo) == Utility::ChannelLeaveCodes::SUCCESS);

				auto testDeleteResult =testTable->Delete()->Where("login = '" + std::string(testNewUserLogin) + "'")->And("email = '" + std::string(testNewUserEmail) + "'")->execute();
				REQUIRE(!testDeleteResult.has_value());

			}
		}

		SECTION("Leave channel")
		{
			SECTION("Leave with invalid channel name")
			{
				auto testBadChannelName{ "g780gqfa80ybfdasfb" };
				Network::ChannelLeaveInfo testBadLeave(testUserID, testChannelID, testBadChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testBadLeave) == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND);
			}

			SECTION("Leave with invalid id")
			{
				auto testBadChannelCreatorID{ 0 };
				Network::ChannelLeaveInfo testBadLeave(testBadChannelCreatorID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testBadLeave) == Utility::ChannelLeaveCodes::CHANNEL_NOT_FOUND);
			}

			SECTION("Leave with valid data")
			{
				Network::ChannelLeaveInfo testLeaveChannel(testUserID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testLeaveChannel) == Utility::ChannelLeaveCodes::SUCCESS);
			}
		}

		SECTION("Delete channel")
		{
			SECTION("Deleting never existing channel")
			{
				SECTION("Invalid channel name")
				{
					auto testBadChannelName{ "asdasdasdasdasdadw213*" };
					Network::ChannelDeleteInfo testBadDeleteInfo(testUserID, testChannelID, testBadChannelName);
					REQUIRE(testChannelRepos.deleteChannel(testBadDeleteInfo) == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND);
				}

				SECTION("Trying to delete channel when user is not owner")
				{
					auto testNotOwnerUserID{ UINT64_MAX };
					Network::ChannelDeleteInfo testBadDeleteInfo(testNotOwnerUserID, testChannelID, testChannelName);
					REQUIRE(testChannelRepos.deleteChannel(testBadDeleteInfo) == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER);
				}
			}

			SECTION("Deleting already existing channel")
			{
				Network::ChannelDeleteInfo testDeleteChannelInfo(testUserID, testChannelID, testChannelName);

				REQUIRE(testChannelRepos.deleteChannel(testDeleteChannelInfo) == Utility::ChannelDeleteCode::SUCCESS);
			}
		}
	}

	SECTION("Direct message repository")
	{
		SECTION("Direct message repos constructor")
		{
			REQUIRE_NOTHROW(DirectMessageRepository(PostgreAdapter::Instance()));
		}

		DirectMessageRepository testDirectMessageRepos(PostgreAdapter::Instance());

		SECTION("Creating direct chat")
		{
			SECTION("Null sender id")
			{
				auto testSenderID{ 0 }, testReceiverID{ 1 };
				REQUIRE(testDirectMessageRepos.addDirectChat(testSenderID, testReceiverID) == Utility::DirectMessageStatus::FAILED);
			}

		}
	}

	SECTION("Delete user")
	{
		testTable->Delete()->Where("login = '" + std::string(testLogin) + "'")->And("email = '" + std::string(testEmail) + "'")->execute();
		auto findUser = testTable->Select()->columns({ "login" })->Where("login = '" + std::string(testLogin) + "'")->execute();
	
		REQUIRE(!findUser.has_value());
	}
}
