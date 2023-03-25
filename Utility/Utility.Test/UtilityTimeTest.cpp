#include <catch2/catch.hpp>
#include <time.h>

#include "Utility/UtilityTime.hpp"

using Catch::Matchers::Contains;
using std::chrono::system_clock;

TEST_CASE("consoleLogTimestamp test")
{
    std::ostringstream outputTime;
    auto               coutBuff = std::cout.rdbuf();  // save pointer to std::cout buffer
    std::cout.rdbuf(outputTime.rdbuf());              // substitute internal std::cout buffer with buffer of 'local' object

    system_clock::time_point currentTime = system_clock::now();
    UtilityTime::consoleLogTimestamp();

    std::cout.rdbuf(coutBuff);  // go back to old buffer

    std::time_t currentTime_time_t = system_clock::to_time_t(currentTime);
    std::tm     currentTime_tm;

#if defined(_MSC_VER)
    localtime_s(&currentTime_tm, &currentTime_time_t);
#elif defined(__unix__)
    localtime_r(&currentTime_tm, &currentTime_time_t);
#else
    static std::mutex           mu;
    std::lock_guard<std::mutex> lock(mu);

    currentTime_tm = *std::localtime(&currentTime_time_t);
#endif

    std::string checkTime(25, '\0');
    std::strftime(checkTime.data(), checkTime.size(), "%Y-%m-%d %H:%M:%S%z", &currentTime_tm);

    REQUIRE_THAT(outputTime.str(), Contains(checkTime.data()));
}
