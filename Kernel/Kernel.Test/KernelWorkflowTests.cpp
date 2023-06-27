#include <catch2/catch.hpp>
#include "Utilities/TestUtility.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;
using Kernel::Builder::SettingsManager;

TEST_CASE("Constructor of the KernelBuilder", "[KernelBuilder]")
{
    SECTION("Constructing with parameters")
    {
        CHECK_NOTHROW(KernelBuilder(std::make_unique<SettingsManager>()));
    }
}

TEST_CASE("SetRepoManager of the KernelBuilder", "[KernelBuilder]")
{
    SECTION("Setting another value")
    {
        KernelBuilder KernelBuilder(std::make_unique<SettingsManager>());

        CHECK_NOTHROW(KernelBuilder.setRepoManager(nullptr));
    }
}

TEST_CASE("Successfull Kernel start up", "[Kernel]")
{
    auto testKernel = makeTestKernel();

    SECTION("Comparison of the specified port with the expected port")
    {
        REQUIRE(std::stoi(configArgs.getKernelPortArguments().second)
            == TestKernelInfo::Port::test);
    }
    CHECK_NOTHROW(testKernel->start());
}

TEST_CASE("Kernel start up with bad port", "[Kernel]")
{   
    using Kernel::Builder::SettingsManager;

    auto      settingsManager = std::make_unique<SettingsManager>(configArgs.getSettings());
    TestKernel testKernel      = KernelBuilder(std::move(settingsManager))
                                          .setRepoManager(getTestDatabase())
                                          .makeKernel();

    SECTION("Comparison of the specified port with the expected bad port")
    {
        REQUIRE_FALSE(std::stoi(configArgs.getBadKernelPortArguments().second)
            == TestKernelInfo::Port::test);
    }
    CHECK_NOTHROW(testKernel->start());
}
