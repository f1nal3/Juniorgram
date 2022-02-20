#pragma once
#include <string_view>

namespace DBOptions
{
    constexpr std::string_view real = 
        "dbname=postgres user=postgres hostaddr=127.0.0.1 port=5432 password=1221909128";

    constexpr std::string_view test =
        "hostaddr=127.0.0.1 port=5432 dbname=testdb user=postgres";
}
