#define __STDC_WANT_LIB_EXT1__ 1

#include <catch2/catch.hpp>
#include <time.h>
#include <conio.h>

#include "Utility/UtilityTime.hpp"

using Catch::Matchers::Contains;
using SysTime= std::chrono::system_clock;

TEST_CASE("consoleLogTimestamp test")
{
    std::ostringstream output_time;
    auto               cout_buff = std::cout.rdbuf();  // save pointer to std::cout buffer
    std::cout.rdbuf(output_time.rdbuf());              // substitute internal std::cout buffer with buffer of 'local' object

    //getting the time
    SysTime::time_point current_time = SysTime::now();
    UtilityTime::consoleLogTimestamp();

    std::cout.rdbuf(cout_buff);                 // go back to old buffer

    std::time_t current_time_time_t = SysTime::to_time_t(current_time);
    std::tm current_time_tm;

    localtime_s(&current_time_tm, &current_time_time_t);

    std::string checkTime(25, '\0');

    std::strftime(checkTime.data(), checkTime.size(), "%Y-%m-%d %H:%M:%S%z", &current_time_tm);

    REQUIRE_THAT(output_time.str(), Contains(checkTime.data()));
}