#include <catch2/catch.hpp>
#include <Network/Connection.hpp>

TEST_CASE("Message comparison operators")
{
	Network::Message early;
    Network::Message late;

    uint64_t         testMilliseconds = 10000;

	early.mHeader.mTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    late.mHeader.mTimestamp  = early.mHeader.mTimestamp + testMilliseconds;

	SECTION("Operator <")
	{
		REQUIRE((early < late) == true);
		REQUIRE((late < early) == false);
	}

	SECTION("Operator >")
    {
        REQUIRE((late > early) == true);
        REQUIRE((early > late) == false);
    }
}

TEST_CASE("Message default constructor")
{
	SECTION("Default constructor")
	{
		std::vector<Network::Message> messages;

		for (int i = 0; i != 10; ++i)
		{
			messages.emplace_back();
		}

		REQUIRE(std::is_sorted(messages.begin(), messages.end()));
	}
}
