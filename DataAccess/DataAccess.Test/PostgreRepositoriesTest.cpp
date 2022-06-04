#include <catch2/catch.hpp>

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
	RegisterRepository testRepos(PostgreAdapter::Instance());

	auto testEmail{ "anotheruser@gmail.com" };
	auto testLogin{ "anotheruser" };
	auto testPassHash{ "65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5" };

	SECTION("Register repository")
	{
		SECTION("Register user") {

			Network::RegistrationInfo ourUser(testEmail, testLogin, testPassHash);
			REQUIRE_NOTHROW(testRepos.registerUser(ourUser));
		}
	}
	SECTION("Delete user")
	{
		auto testAdapter = PostgreAdapter::Instance();
		auto testTable = std::make_unique<PostgreTable>("users", testAdapter);

		testTable->Delete()->Where("login = '" + std::string(testLogin) + "'")->And("email = '" + std::string(testEmail) + "'")->execute();
		//auto deleteResult = testTable->Select()->columns({ "COUNT(*)" })->Where("login = " + std::string(testLogin))->And("email = " + std::string(testEmail))->execute();
		auto findUser = testTable->Select()->columns({ "login" })->Where("login = '" + std::string(testLogin) + "'")->execute();

		REQUIRE(findUser.has_value() == 0);
	}

}