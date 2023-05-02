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
using DataAccess::PGQueryBuilder;
using DataAccess::RepliesRepository;

using Utility::ChannelCreateCodes;
using Utility::ChannelDeleteCode;
using Utility::ChannelLeaveCodes;
using Utility::ChannelSubscribingCodes;
using Utility::RegistrationCodes;
using Utility::DirectMessageStatus;
using Utility::StoringMessageCodes;
using Utility::DeletingMessageCodes;
using Utility::EditingMessageCodes;
using Utility::ReactionMessageCodes;
using Utility::StoringReplyCodes;

TEST_CASE("PostgreRepositories test", "[dummy]")
{
	auto testEmail{ "anotheruser@gmail.com" };
	auto testLogin{ "anotheruser" };
	auto testPassHash{ "65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5" };

	auto testTable = std::make_unique<PGQueryBuilder>("users", PostgreAdapter::Instance(DBOptions::test));

	SECTION("Register repository")
	{
		SECTION("Register repos constructor")
		{
			REQUIRE_NOTHROW(RegisterRepository(PostgreAdapter::Instance(DBOptions::test)));
		}

		SECTION("Register repos constructor but reference already used")
		{
			REQUIRE_NOTHROW(RegisterRepository(PostgreAdapter::Instance()));
		}

		RegisterRepository testRegisterRepos(PostgreAdapter::Instance(DBOptions::test));

		SECTION("Register user")
		{
			Models::RegistrationInfo ourUser(testEmail, testLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(ourUser) == RegistrationCodes::SUCCESS);
		}

		SECTION("Trying to register new user with data of already existing user")
		{
			auto newTestLogin{ "newuser" };
			Models::RegistrationInfo badUserEmail(testEmail, newTestLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(badUserEmail) == RegistrationCodes::EMAIL_ALREADY_EXISTS);

			auto newTestEmail{ "newuser@gmail.com" };
			Models::RegistrationInfo badUserLogin(newTestEmail, testLogin, testPassHash);
			REQUIRE(testRegisterRepos.registerUser(badUserLogin) == RegistrationCodes::LOGIN_ALREADY_EXISTS);
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
			Models::LoginInfo testUser(testLogin, testPassHash);

			SECTION("Valid login")
			{
				REQUIRE(testLoginRepos.loginUser(testUser) == testUserID[0][0].as<uint64_t>());
				REQUIRE_NOTHROW(testLoginRepos.loginUser(testUser));
			}
		}

		SECTION("Let's try to login with invalid values")
		{
			auto testBadHash{ "asdt24rersdf*_fs9dfs" };
			Models::LoginInfo testBadUser(testLogin, testBadHash);
			REQUIRE(testLoginRepos.loginUser(testBadUser) == 0);

			REQUIRE_NOTHROW(testLoginRepos.loginUser(testBadUser));
		}

		SECTION("Waiting an exception, but it have been catchen by try-catch block")
		{
			Models::LoginInfo testBadUser("nooneexist*-_123&^", testPassHash);

			REQUIRE_NOTHROW(testLoginRepos.loginUser(testBadUser));
		}
	}

	SECTION("Channels repository and other repositories connected pretty close to channels")
	{
		auto testUserID = testTable->Select()->columns({ "id" })
			->Where("login = '" + std::string(testLogin) + "'")
			->execute().value()[0][0].as<uint64_t>();

		ChannelsRepository testChannelRepos(PostgreAdapter::Instance());
		auto testChannelName{ "testChannel" };
		///if we have no one channel in DB -> return 1 as ID of channel, else -> return lastID
		auto testChannelID = testChannelRepos.getAllChannelsList().empty() ? 1 : (testChannelRepos.getAllChannelsList().back()._channelID);

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
			Models::ChannelInfo testChannel(testUserID, testChannelID, testChannelName);

			SECTION("Create new channel with valid data")
			{
				REQUIRE(testChannelRepos.createChannel(testChannel) == ChannelCreateCodes::SUCCESS);
			}			

			SECTION("Let's try to create channel with data of already existing channel")
			{
				REQUIRE(testChannelRepos.createChannel(testChannel) == ChannelCreateCodes::CHANNEL_ALREADY_CREATED);
			}

			SECTION("Try to make an error")
			{
				auto testChannelBadID{ 0 };
				auto testChannelBadName{ "" };
				Models::ChannelInfo testBadChannel(0, testChannelBadID, testChannelBadName);
				REQUIRE_THROWS(testChannelRepos.createChannel(testBadChannel));
			}
		}

		SECTION("Get all channels but at least one must exist")
		{
			REQUIRE_FALSE(testChannelRepos.getAllChannelsList().empty());
		}

		SECTION("Let's get channels subscription list")
		{
			REQUIRE_FALSE(testChannelRepos.getChannelSubscriptionList(testUserID).empty());
		}

		SECTION("Subcribe to channel")
		{
			Models::ChannelSubscriptionInfo testSubsInfo(testChannelID);
			testSubsInfo._userID = testUserID;

			SECTION("What if our user has already signed to channel")
			{
				REQUIRE(testChannelRepos.subscribeToChannel(testSubsInfo) == ChannelSubscribingCodes::CHANNEL_HAS_ALREADY_BEEN_SIGNED);
			}

			SECTION("Create new user and sign it to our channel")
			{
				RegisterRepository testRegNewUser(PostgreAdapter::Instance());

				auto testNewUserHash{ "asdadagfdgdfgdfgdfg" };
				auto testNewUserLogin{ "seconduser" };
				auto testNewUserEmail{ "seconduser@gmail.com" };

				Models::RegistrationInfo testRegNewInfo(testNewUserEmail, testNewUserLogin, testNewUserHash);
				testRegNewUser.registerUser(testRegNewInfo);
				testSubsInfo._userID = testTable->Select()->columns({ "id" })->Where("login ='" + std::string(testNewUserLogin) + "'")->execute().value()[0][0].as<uint16_t>();

				REQUIRE(testChannelRepos.subscribeToChannel(testSubsInfo) == ChannelSubscribingCodes::SUCCESS);

				Models::ChannelLeaveInfo testNewLeaveInfo(testSubsInfo._userID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testNewLeaveInfo) == ChannelLeaveCodes::SUCCESS);

				auto testDeleteResult = testTable->Delete()->Where("login = '" + std::string(testNewUserLogin) + "'")->And("email = '" + std::string(testNewUserEmail) + "'")->execute();
				REQUIRE_FALSE(testDeleteResult.has_value());
			}
		}

		SECTION("Message repository")
		{
			SECTION("Message repos constructor")
			{
				REQUIRE_NOTHROW(MessagesRepository(PostgreAdapter::Instance()));
			}

			MessagesRepository testMessageRepos(PostgreAdapter::Instance());

			auto testMessageText{ "Hello everyone!" };
			Models::MessageInfo testMessage(testChannelID, testMessageText);
			testMessage._senderID = testUserID;

			SECTION("We've stored message, let's check our message history")
			{
				SECTION("Message history but it's empty")
				{
					REQUIRE(testMessageRepos.getMessageHistory(testChannelID).empty());
				}

				SECTION("Success storing")
				{
					REQUIRE(testMessageRepos.storeMessage(testMessage) == StoringMessageCodes::SUCCESS);
				}

				SECTION("Functionality with message")
				{
					REQUIRE_FALSE(testMessageRepos.getMessageHistory(testChannelID).empty());
				}

				#if defined(_MSC_VER)
				SECTION("Storing message but channel does not exist")
				{
					REQUIRE_THROWS(testMessageRepos.storeMessage(Models::MessageInfo(0, testMessageText)));
				}
				#endif

				SECTION("Try to edit the message")
				{
					testTable->changeTable("msgs");

					testMessage._msgID = testTable->Select()
						->columns({ "*" })
						->Where("sender_id = " + std::to_string(testUserID))
						->And("msg ='" + std::string(testMessageText) + "'")
						->execute().value()[0][0].as<uint64_t>();

					REQUIRE(testMessageRepos.editMessage(testMessage) == EditingMessageCodes::SUCCESS);

					testTable->changeTable("users");
				}

				SECTION("Try to edit message which does not exist")
				{
					testMessage._senderID = 0;
					REQUIRE(testMessageRepos.editMessage(testMessage) == EditingMessageCodes::FAILED);
					testMessage._senderID = testUserID;
				}

				SECTION("Update message reactions")
				{
					SECTION("Fail to update message reactions")
					{
						testMessage._reactions.clear();
						REQUIRE(testMessageRepos.updateMessageReactions(testMessage) == ReactionMessageCodes::FAILED);
					}

					SECTION("Update reactions with invalid stickers")
					{
						REQUIRE(testMessageRepos.updateMessageReactions(testMessage) == ReactionMessageCodes::FAILED);
						testMessage._reactions.clear();
					}

					SECTION("Almost good sticker, but id does not exist")
					{
						testMessage._reactions.emplace(std::pair<uint32_t, uint32_t>(8, std::numeric_limits<std::uint32_t>::max()));
						REQUIRE(testMessageRepos.updateMessageReactions(testMessage) == ReactionMessageCodes::FAILED);
					}

					SECTION("Another sticker to our message")
					{
						testMessage._reactions.emplace(std::pair<uint32_t, uint32_t>(2, std::numeric_limits<std::uint32_t>::max()));
						REQUIRE(testMessageRepos.updateMessageReactions(testMessage) == ReactionMessageCodes::SUCCESS);
					}

					SECTION("Let's delete reaction from message")
					{
						testMessage._reactions.emplace(std::pair<uint32_t, uint32_t>(2, std::numeric_limits<std::uint32_t>::max()));
						REQUIRE(testMessageRepos.updateMessageReactions(testMessage) == ReactionMessageCodes::SUCCESS);
					}
				}

				SECTION("Replies repository")
				{
					SECTION("Reply repos constructor")
					{
						REQUIRE_NOTHROW(RepliesRepository(PostgreAdapter::Instance()));
					}

					RepliesRepository testReplyRepos(PostgreAdapter::Instance());

					SECTION("Let's get empty reply history")
					{
						REQUIRE(testReplyRepos.getReplyHistory(testChannelID).empty());
					}

					SECTION("Let's store reply")
					{
						auto testReplyText{ "Hello to you" };
						Models::ReplyInfo testReply(testChannelID, testReplyText);

						SECTION("Bad reply")
						{
							REQUIRE(testReplyRepos.storeReply(testReply) == StoringReplyCodes::FAILED);
						}

						testTable->changeTable("msgs");

						testReply._msgID = testTable->Select()
							->columns({ "*" })
							->Where("sender_id = " + std::to_string(testUserID))
							->And("msg ='" + std::string(testMessageText) + "'")
							->execute().value()[0][0].as<uint64_t>();

						testReply._senderID = testUserID;
						testReply._userLogin = testLogin;
						testReply._msgIdOwner = testUserID;

						testTable->changeTable("users");

						SECTION("Good reply")
						{
							REQUIRE(testReplyRepos.storeReply(testReply) == StoringReplyCodes::SUCCESS);
						}

						SECTION("Mostly good reply but channel does not exist")
						{
							testReply._channelID = 0;
							REQUIRE(testReplyRepos.storeReply(testReply) == StoringReplyCodes::FAILED);
						}

						SECTION("Get not empty reply history")
						{
							REQUIRE_FALSE(testReplyRepos.getReplyHistory(testChannelID).empty());
						}
					}
				}
			}

			SECTION("Let's delete message")
			{
				SECTION("Delete already existing message")
				{
					Models::MessageInfo testDeleteMessage(testChannelID, testMessageText);

					REQUIRE(testMessageRepos.deleteMessage(testDeleteMessage) == DeletingMessageCodes::SUCCESS);
				}

				SECTION("Let's delete message which does not exist, but our function works perfectly")
				{
					Models::MessageInfo testDeleteBadMessage(testChannelID, testMessageText);
					testDeleteBadMessage._msgID = 0;

					REQUIRE(testMessageRepos.deleteMessage(testDeleteBadMessage) == DeletingMessageCodes::SUCCESS);
				}
			}
		}

		SECTION("Leave channel")
		{
			SECTION("Leave with invalid channel name")
			{
				auto testBadChannelName{ "g780gqfa80ybfdasfb" };
				Models::ChannelLeaveInfo testBadLeave(testUserID, testChannelID, testBadChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testBadLeave) == ChannelLeaveCodes::CHANNEL_NOT_FOUND);
			}

			SECTION("Leave with invalid id")
			{
				auto testBadChannelCreatorID{ 0 };
				Models::ChannelLeaveInfo testBadLeave(testBadChannelCreatorID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testBadLeave) == ChannelLeaveCodes::CHANNEL_NOT_FOUND);
			}

			SECTION("Leave with valid data")
			{
				Models::ChannelLeaveInfo testLeaveChannel(testUserID, testChannelID, testChannelName);
				REQUIRE(testChannelRepos.leaveChannel(testLeaveChannel) == ChannelLeaveCodes::SUCCESS);
			}
		}

		SECTION("Delete channel")
		{
			SECTION("Deleting never existing channel")
			{
				SECTION("Invalid channel name")
				{
					auto testBadChannelName{ "asdasdasdasdasdadw213*" };
					Models::ChannelDeleteInfo testBadDeleteInfo(testUserID, testChannelID, testBadChannelName);
					REQUIRE(testChannelRepos.deleteChannel(testBadDeleteInfo) == Utility::ChannelDeleteCode::CHANNEL_NOT_FOUND);
				}

				SECTION("Trying to delete channel when user is not owner")
				{
					auto testNotOwnerUserID{ UINT64_MAX };
					Models::ChannelDeleteInfo testBadDeleteInfo(testNotOwnerUserID, testChannelID, testChannelName);
					REQUIRE(testChannelRepos.deleteChannel(testBadDeleteInfo) == Utility::ChannelDeleteCode::CHANNEL_IS_NOT_USER);
				}
			}

			SECTION("Deleting already existing channel")
			{
				MessagesRepository testRepos(PostgreAdapter::Instance());
				Models::MessageInfo testMessage(testChannelID, "Hello");
				
				testTable->changeTable("msgs");


				testMessage._senderID = testUserID;
				testRepos.storeMessage(testMessage);

				testMessage._msgID = testTable->Select()
					->columns({ "*" })
					->Where("sender_id = " + std::to_string(testUserID))
					->And("msg ='" + std::string("Hello") + "'")
					->execute().value()[0][0].as<uint64_t>();
				

				Models::ChannelDeleteInfo testDeleteChannelInfo(testUserID, testChannelID, testChannelName);

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
				auto testSenderID{ 0 };
				auto testReceiverID{ 1 };

				REQUIRE(testDirectMessageRepos.addDirectChat(testSenderID, testReceiverID) == DirectMessageStatus::FAILED);
			}

			SECTION("Try to make it failed")
			{
				auto testSenderID{ 1 };
				auto testReceiverID{ 2 };

				REQUIRE_THROWS(testDirectMessageRepos.addDirectChat(testSenderID, testReceiverID));
			}

			SECTION("Another failed creating")
			{
				auto testSenderID{ 1 };
				auto testReceiverID{ 0 };

				REQUIRE_THROWS(testDirectMessageRepos.addDirectChat(testSenderID, testReceiverID));
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
