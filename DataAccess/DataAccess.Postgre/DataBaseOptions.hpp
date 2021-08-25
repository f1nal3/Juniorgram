#pragma once
#include <string_view>

namespace DBOptions
{
    constexpr std::string_view real = 
        "dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432 password=123";

    constexpr std::string_view test =
                "hostaddr=127.0.0.1 port=5432 dbname=testdb user=postgres";
}
