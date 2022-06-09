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

	auto testTable = std::make_unique<PostgreTable>("users", PostgreAdapter::Instance(DBOptions::test));

	SECTION("Register repository")
	{
		SECTION("Register repos constructor")
		{
			REQUIRE_NOTHROW(RegisterRepository(PostgreAdapter::Instance()));
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
		SECTION("Channels repos constructor")
		{
			REQUIRE_NOTHROW(ChannelsRepository(PostgreAdapter::Instance()));
		}

		ChannelsRepository testChannelRepos(PostgreAdapter::Instance());

		SECTION("getAllChannels but no one exists")
		{
			REQUIRE(testChannelRepos.getAllChannelsList().size() == 0);
		}

		SECTION("Create channel")
		{
			auto testUserID = testTable->Select()->columns({ "id" })
				Network::ChannelInfo testChannel(, 1, "testChannel");
		}

		SECTION("Get all channels")
		{
			REQUIRE_NOTHROW(testChannelRepos.getAllChannelsList());

		}
	}

	SECTION("Delete user")
	{
		auto testAdapter = PostgreAdapter::Instance();
		auto testTable = std::make_unique<PostgreTable>("users", testAdapter);

		testTable->Delete()->Where("login = '" + std::string(testLogin) + "'")->And("email = '" + std::string(testEmail) + "'")->execute();
		auto findUser = testTable->Select()->columns({ "login" })->Where("login = '" + std::string(testLogin) + "'")->execute();

		REQUIRE(findUser.has_value() == 0);
	}

}
