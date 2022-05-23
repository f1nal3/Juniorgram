#include <catch2/catch.hpp>

#include "Utility/Exception.hpp"

using Catch::Contains;

[[noreturn]] static void ExceptionThrowerMock(){
    throw Utility::NotImplementedException("Not implemented", __FILE__, __LINE__);
}

[[noreturn]] static void OperationDBExceptionMock() { 
    throw Utility::OperationDBException("DB error operation", __FILE__, __LINE__); 
}

TEST_CASE("Test NotImplementedException format", "[Utility.Exception]")
{
    REQUIRE_THROWS_WITH(ExceptionThrowerMock(), Contains("Not implemented") && Contains("ExceptionTest.cpp"));

}

TEST_CASE("Test OperationDBException format", "[Utility.Exception]")
{
    REQUIRE_THROWS_WITH(OperationDBExceptionMock(), Contains("DB error operation") && Contains("ExceptionTest.cpp"));
}
