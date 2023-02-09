#pragma once 

#include <string_view>

namespace TestUtility
{
constexpr std::string_view testProperties
{
	"hostaddr=127.0.0.1 port=5432 dbname=mockdb user=tester"
};
} // namespace TestUtility
