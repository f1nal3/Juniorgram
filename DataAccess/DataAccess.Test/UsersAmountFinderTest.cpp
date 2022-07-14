#include "catch2/catch.hpp"

#include <DataAccess.Postgre/UsersAmountFinder.hpp>
#include <DataAccess.Postgre/PostgreRepositories.hpp>

using DataAccess::PostgreTable;
using DataAccess::PostgreAdapter;
using DataAccess::RegisterRepository;

TEST_CASE("UsersAmountFinder", "[dummy]")
{
	SECTION("UsersAmountFinder constructor")
	{
		REQUIRE_NOTHROW(UsersAmountFinder());
	}

	auto testTable = std::make_unique<PostgreTable>("users", PostgreAdapter::Instance(DBOptions::test));
	UsersAmountFinder testFinder;
	auto testCondition{ "login = 'anotheruser'" };

	SECTION("Find users with the same attribute but no users")
	{
		REQUIRE(testFinder.findUsersAmountWithSameTableAttribute(testCondition) == 0);
	}

	auto testEmail{ "anotheruser@gmail.com" };
	auto testLogin{ "anotheruser" };
	auto testPassHash{ "65e84be33532fb784c48129675f9eff3a682b27168c0ea744b2cf58ee02337c5" };

	SECTION("Find users with the same login but they don't exist")
	{
		REQUIRE(testFinder.findUsersAmountWithSameLogin(testLogin) == 0);
	}

	SECTION("Find users with the same email but the don't exist")
	{
		REQUIRE(testFinder.findUsersAmountWithSameEmail(testEmail) == 0);
	}

	Models::RegistrationInfo testRegInfo(testEmail, testLogin, testPassHash);

	SECTION("Find users with same all data")
	{
		REQUIRE(testFinder.findUsersAmountWithAllSameData(testRegInfo) == 0);
	}

	RegisterRepository testRegRepos(PostgreAdapter::Instance());
	testRegRepos.registerUser(testRegInfo);

	SECTION("Find users with the same attribute")
	{
		REQUIRE(testFinder.findUsersAmountWithSameTableAttribute(testCondition) == 1);
	}

	SECTION("Find users with the same login but they don't exist")
	{
		REQUIRE(testFinder.findUsersAmountWithSameLogin(testLogin) == 1);
	}

	SECTION("Find users with the same email but the don't exist")
	{
		REQUIRE(testFinder.findUsersAmountWithSameEmail(testEmail) == 1);
	}

	SECTION("Find users with same all data")
	{
		REQUIRE(testFinder.findUsersAmountWithAllSameData(testRegInfo) == 1);
	}

	testTable->Delete()->Where("login = '" + std::string(testLogin) + "'")->And("email = '" + std::string(testEmail) + "'")->execute();
	auto findUser = testTable->Select()->columns({ "login" })->Where("login = '" + std::string(testLogin) + "'")->execute();

	REQUIRE_FALSE(findUser.has_value());
}
