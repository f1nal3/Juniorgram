#pragma once 

#include<string_view>

namespace TestDBOptions
{
constexpr std::string_view testProperties
{
	"hostaddr=127.0.0.1 port=5432 dbname=testdb user=postgres"
};
}