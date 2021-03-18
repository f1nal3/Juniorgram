#include <catch2/catch.hpp>
#include "../Include/Network/Server.hpp"

TEST_CASE("Message comparison operators")
{
	network::Message early;
    network::Message sameOne;
	network::Message sameTwo;
    network::Message late;

	std::chrono::time_point<std::chrono::system_clock> timePoint = std::chrono::system_clock::now();

	early.mHeader.mTimestamp = timePoint - std::chrono::milliseconds(10);
	sameOne.mHeader.mTimestamp = timePoint;
	sameTwo.mHeader.mTimestamp = timePoint;
	late.mHeader.mTimestamp = timePoint + std::chrono::milliseconds(10);

	SECTION("Operator ==")
	{
		REQUIRE((early == late) == false);
		REQUIRE((sameOne == sameTwo) == true);
	}

	SECTION("Operator <")
	{
		REQUIRE((early < late) == true);
		REQUIRE((sameOne < sameTwo) == false);
	}

	SECTION("Operator >")
    {
        REQUIRE((late > early) == true);
        REQUIRE((sameOne > sameTwo) == false);
    }

	SECTION("Operator <=")
    {
        REQUIRE((early <= late) == true);
        REQUIRE((sameOne <= sameTwo) == true);
    }

	SECTION("Operator >=")
    {
        REQUIRE((late >= early) == true);
        REQUIRE((sameOne >= sameTwo) == true);
    }
}

TEST_CASE("Message default constructor")
{
	SECTION("Default constructor")
	{
		std::vector<network::Message> messages;

		for (int i = 0; i != 10; ++i)
		{
			messages.emplace_back();
		}

		REQUIRE(std::is_sorted(messages.begin(), messages.end()));
	}
}
