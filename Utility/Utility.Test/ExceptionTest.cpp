#include <catch2/catch.hpp>

#include "Utility/Exception.hpp"

using Catch::Contains;

[[noreturn]] static void ExceptionThrowerMock()
{
    throw Utility::NotImplementedException("Not implemented", __FILE__, __LINE__);
}

void NoExceptionMock()
{
    Utility::NotImplementedException("Implemented", __FILE__, __LINE__);
}

[[noreturn]] static void OperationDBExceptionMock()
{
    throw Utility::OperationDBException("DB error operation", __FILE__, __LINE__); 
}

void OperationDBNoExceptionMock()
{
    Utility::OperationDBException("DB success operation", __FILE__, __LINE__);
}

[[noreturn]] static void LoggingExceptionMock()
{ 
    throw Utility::LoggingException("Something went wrong", __FILE__, __LINE__);
}

void LoggingNoExceptionMock()
{ 
    Utility::LoggingException("Logging without throw", __FILE__, __LINE__);
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

TEST_CASE("Test LoggingException format", "[Utility.Exception]")
{
    SECTION("Exception is expected")
    {
        REQUIRE_THROWS_WITH(LoggingExceptionMock(), Contains("Something went wrong") && Contains("ExceptionTest.cpp"));
    }

    SECTION("Exception is not expected") 
    { 
        REQUIRE_NOTHROW(LoggingNoExceptionMock()); 
    }
}

TEST_CASE("Test formatExceptionMessage")
{
    std::string message  = "Test exception";
    std::string filename = "ExceptionTest.txt";
    uint16_t    line     = 3;

    REQUIRE_THAT(Utility::formatExceptionMessage(message, filename, line),
                 Contains(message.data()) && Contains(filename.data()) && Contains(std::to_string(line).data()));

    REQUIRE_NOTHROW(Utility::formatExceptionMessage(message, filename, line));
}
