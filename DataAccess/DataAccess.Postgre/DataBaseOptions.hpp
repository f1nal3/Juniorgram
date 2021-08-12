#pragma once
#include <string_view>

namespace DBOptions
{
    constexpr std::string_view real = 
        "dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432";

    constexpr std::string_view test =
                "hostaddr=104.40.239.183 port=5432 dbname=test user=test_user password=qwerty";
}
