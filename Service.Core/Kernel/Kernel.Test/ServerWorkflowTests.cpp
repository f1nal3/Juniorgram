#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Successfull Kernel start up", "[Kernel][Success]")
{
    auto testKernel = makeTestKernel();

    SECTION("Comparison of the specified port with the expected port")
    {
        REQUIRE(std::stoi(configArgs.getKernelPortArguments().second) 
            == TestKernelInfo::Port::test);
    }
    CHECK_NOTHROW(testKernel->start());
}

TEST_CASE("Kernel start up with bad port", "[Kernel][Failed]")
{   
    TestKernel testKernel = KernelBuilder()
                                .setValue(configArgs.getBadKernelPortArguments())
                                .setValue(configArgs.getDatabaseArguments())
                                .setValue(configArgs.getHostAddrArguments())
                                .setValue(configArgs.getDatabasePortArguments())
                                .setValue(configArgs.getDatabaseUserArguments())
                                .setValue(configArgs.getDatabasePasswordArguments())
                                .setValue(getTestDatabase().release())
                                .makeKernel();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadKernelPortArguments().second) 
            == TestKernelInfo::Port::test);
    }
    CHECK_NOTHROW(testKernel->start());
}
