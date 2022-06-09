#include <catch2/catch.hpp>
#include <ctime>

#include "Utility/UtilityTime.hpp"

using Catch::Matchers::Contains;

TEST_CASE("consoleLogTimestamp test")
{
    std::ostringstream output_time;
    auto               cout_buff = std::cout.rdbuf();  // save pointer to std::cout buffer
    std::cout.rdbuf(output_time.rdbuf());              // substitute internal std::cout buffer with buffer of 'local' object

    //getting the time
    std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
    UtilityTime::consoleLogTimestamp();

    std::cout.rdbuf(cout_buff);                 // go back to old buffer

    std::chrono::system_clock::time_point  now    = std::chrono::system_clock::now();
    std::time_t                            now_time_t  = std::chrono::system_clock::to_time_t(now);
    std::tm                                now_tm     = *std::localtime(&now_time_t);

    char                                   checkTime[26];

    std::strftime(checkTime, sizeof(checkTime), "%Y-%m-%d %H:%M:%S%z", &now_tm);

    REQUIRE_THAT(output_time.str(), Contains(checkTime));
}