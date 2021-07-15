#include <array>
#include <catch2/catch.hpp>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/Primitives.hpp"
#include <Utility.Static/YasSerializer.hpp>

using namespace Utility;
using namespace Network;

TEST_CASE("Test binary serialization & deserialization of plain types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of int type")
    {
        const int serializedValue = 10;
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        int deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of double type")
    {
        const double serializedValue = 10.9100;
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        double deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE((serializedValue - 0.005) < deserializedValue);
        REQUIRE((serializedValue + 0.005) > deserializedValue);
    }
}

TEST_CASE("Test binary serialization & deserialization of complex types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of std::string")
    {
        const std::string serializedValue = "hello, I'm from India!";
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::string deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::chrono::time_point")
    {
        const std::chrono::time_point<std::chrono::system_clock> serializedValue =
            std::chrono::system_clock::now();
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::chrono::time_point<std::chrono::system_clock> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::vector<uint32_t>")
    {
        const std::vector<std::uint32_t> serializedValue = {89, 129, 23, 0, 12, 324};
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::vector<std::uint32_t> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::array<std::string>")
    {
        const std::array<std::string, 3> serializedValue = {"freedom ", "for ", "parrots!"};
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::array<std::string, 3> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::map<char, long int>")
    {
        const std::map<char, long int> serializedValue = {
            {'a', 1000000000}, {'b', -2000000000}, {'c', 3000000000}};
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::map<char, long int> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::list<bool>")
    {
        std::list<bool> serializedValue = {true, false, false, true, false, true, true};
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::list<bool> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::tuple<uint8_t, int, std::wstring>")
    {
        std::tuple<std::uint8_t, int, std::wstring> serializedValue =
            std::make_tuple(static_cast<std::uint8_t>(1), -12345, std::to_wstring(2374.234));
        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::tuple<std::uint8_t, int, std::wstring> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }
}

TEST_CASE("Test binary serialization & deserialization of custom types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of custom type MessageHeader")
    {
        Message::MessageHeader serializedValue;
        serializedValue.mTimestamp   = std::chrono::system_clock::now();
        serializedValue.mMessageType = Message::MessageType::ChannelListRequest;
        serializedValue.mBodySize    = 30;

        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        Message::MessageHeader deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue.mTimestamp == deserializedValue.mTimestamp);
        REQUIRE(serializedValue.mMessageType == deserializedValue.mMessageType);
        REQUIRE(serializedValue.mBodySize == deserializedValue.mBodySize);
    }

    SECTION("Checking serialization & deserialization of custom type ChannelInfo")
    {
        ChannelInfo serializedValue(2672, "kittens");

        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        ChannelInfo deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of custom type MessageInfo")
    {
        MessageInfo serializedValue(123, "I'd like ice-cream=)");

        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        MessageInfo deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type ChannelInfo")
    {
        std::vector<ChannelInfo> serializedValue{ChannelInfo(2672, "kittens"),
                                                          ChannelInfo(1445, "puppies"),
                                                          ChannelInfo(2345, "hamsters")};

        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::vector<ChannelInfo> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type MessageInfo")
    {
        std::vector<MessageInfo> serializedValue{
            MessageInfo(123, "I'd like ice-cream=)"),
            MessageInfo(123, "I'd like pizza=)"),
            MessageInfo(123, "I'd like cheeseburger=)"),
            MessageInfo(123, "I'd like naggets=)")};

        yas::shared_buffer buffer;
        SerializedState state = YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        std::vector<MessageInfo> deserializedValue;
        state = YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }
}
