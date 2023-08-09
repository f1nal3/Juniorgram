#include <catch2/catch.hpp>
#include "TestUtils/TestUtility.hpp"
#include "TestUtils/WaitForTime.hpp"

using namespace TestUtility;
using TestUtility::MessageBody;

TEST_CASE("Default request procedures", "[Kernel][Success]")
{
    auto testKernel = makeTestKernel();
    testKernel->start();

    TestClient client;
    client.connectToKernel(TestKernelInfo::Address::local, TestKernelInfo::Port::test);
    
    SECTION("Successful request to check a ping")
    {
        Message     validMessage;
        const auto& messageInstance = makeMessage(validMessage, 
            MessageType::ServicePing, MessageBody::ValidBody);
        
        client.send(messageInstance);
        testKernel->update();

        WaitForTime waiter(std::chrono::milliseconds(1000));
        waiter.wait();

        REQUIRE(client.getMessageResult().back() ==
             Client::TestObject::MessageResult::Success);
        testKernel->stop();
    }
}
