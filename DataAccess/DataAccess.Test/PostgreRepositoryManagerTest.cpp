#include <catch2/catch.hpp>

#include "DataAccess.Postgre/PostgreRepositoryManager.hpp"

TEST_CASE("fmt method", "[dummy]")
{
	SECTION("Checking function fmt()")
	{
		using namespace DataAccess;

		auto testVariable{ 5 };
		REQUIRE_NOTHROW(fmt<uint16_t>(testVariable));
	}
}

TEST_CASE("RepositoryRequest", "[dummy]")
{
	using DataAccess::RepositoryRequest;
	using DataAccess::ePriority;
	using DataAccess::RequestTask;

	RequestTask testTask;

	SECTION("ReposRequest constructor")
	{
		REQUIRE_NOTHROW(RepositoryRequest(ePriority::_1, testTask));
	}

	RepositoryRequest testReposReq(ePriority::_10, testTask);

	SECTION("Move constructor")
	{
		REQUIRE_NOTHROW(RepositoryRequest(std::move(testReposReq)));
	}

	SECTION("Move assignment")
	{
		RepositoryRequest testRepos(ePriority::_11, testTask);
		REQUIRE_NOTHROW(testRepos = std::move(testReposReq));
	}

	SECTION("getFutureFromTask check, but no state")
	{
		REQUIRE_THROWS(testReposReq.getFutureFromTask());
	}

	SECTION("Operator '<'")
	{
		RepositoryRequest testRepos(ePriority::_10, testTask);
		REQUIRE_NOTHROW(testRepos < testReposReq);
	}

	SECTION("Operator '()', but no state")
	{
		REQUIRE_THROWS(testReposReq());
	}
}

TEST_CASE("FutureResult", "[dummy]")
{
	using DataAccess::FutureResult;
	using DataAccess::RawFuture;

	RawFuture testRawFuture;
	
	SECTION("FutureResult constructor")
	{
		REQUIRE_NOTHROW(FutureResult<uint16_t>(std::move(testRawFuture)));
	}

	FutureResult<uint16_t> testFutureResult(std::move(testRawFuture));

	SECTION("Checking method get()")
	{
		REQUIRE_THROWS(testFutureResult.get());
	}
}

TEST_CASE("PostgreRepositoryManager", "[dummy]")
{
	using DataAccess::PostgreRepositoryManager;
	using DataAccess::PostgreAdapter;

	SECTION("PostgreRepositoryManager constructor")
	{
		//REQUIRE_NOTHROW(PostgreRepositoryManager(PostgreAdapter::Instance()));
	}

}
