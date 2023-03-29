#include <catch2/catch.hpp>
#include <time.h>

#include "Utility/UtilityTime.hpp"

using Catch::Matchers::Contains;
using std::chrono::system_clock;

TEST_CASE("consoleLogTimestamp test")
{
    std::ostringstream outputTime;
    auto               coutBuff = std::cout.rdbuf();    // save pointer to std::cout buffer
    std::cout.rdbuf(outputTime.rdbuf());    // substitute internal std::cout buffer with buffer of 'local' object

    system_clock::time_point bufferTime = system_clock::now();
    UtilityTime::consoleLogTimestamp();

    std::cout.rdbuf(coutBuff);  // go back to old buffer

    std::time_t currentTime   = system_clock::to_time_t(bufferTime);
    std::tm     formattedTime = UtilityTime::safe_localtime(currentTime);

    std::string checkTime(20, '\0');
    std::strftime(checkTime.data(), checkTime.size(), "%Y-%m-%d %H:%M:%S%z", &formattedTime);

    REQUIRE_THAT(outputTime.str(), Contains(checkTime.data()));
}
