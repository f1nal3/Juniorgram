#include <catch2/catch.hpp>

#include "Utility/Exception.hpp"

using Catch::Contains;

[[noreturn]] static void ExceptionThrowerMock(){
    throw Utility::NotImplementedException("Not implemented", __FILE__, __LINE__);
}

[[noreturn]] static void NoExceptionMock() {
    Utility::NotImplementedException("Implemented", __FILE__, __LINE__);
}

[[noreturn]] static void OperationDBExceptionMock() { 
    throw Utility::OperationDBException("DB error operation", __FILE__, __LINE__); 
}

[[noreturn]] static void OperationDBNoExceptionMock() {
    Utility::OperationDBException("DB success operation", __FILE__, __LINE__);
}

TEST_CASE("Test NotImplementedException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(ExceptionThrowerMock(), Contains("Not implemented") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(NoExceptionMock());
    }
}

TEST_CASE("Test OperationDBException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(OperationDBExceptionMock(), Contains("DB error operation") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected")
    {
        REQUIRE_NOTHROW(OperationDBNoExceptionMock());
    }
}
