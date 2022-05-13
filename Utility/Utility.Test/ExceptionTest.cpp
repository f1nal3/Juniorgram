#include <catch2/catch.hpp>

#include "Utility/Exception.hpp"

using Catch::Contains;

[[noreturn]] static void ExceptionThrowerMock()
{
    throw Utility::NotImplementedException("Not implemented", __FILE__, __LINE__);
}

TEST_CASE("Test NotImplementedException format", "[Utility.Exception]")
{
    REQUIRE_THROWS_WITH(ExceptionThrowerMock(), Contains("Not implemented") && Contains("ExceptionTest.cpp"));

}
