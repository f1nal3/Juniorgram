#pragma once 

#include <string_view>

namespace MockObject
{
/**
* @brief Mock properties for connecting to database.
*/
constexpr std::string_view TestProperties
{
	"hostaddr=127.0.0.1 port=5432 dbname=mockdb user=tester"
};
} /// namespace TestUtility
