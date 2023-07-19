#include <array>
#include <catch2/catch.hpp>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/YasSerializer.hpp"
#include <Models/Primitives.hpp>


TEST_CASE("Test binary serialization & deserialization of plain types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of int type")
    {
        const int serializedValue = 10;
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        int deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of double type")
    {
        const double serializedValue = 10.9100;
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        double deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

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
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::string deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::chrono::time_point")
    {
        const std::chrono::time_point<std::chrono::system_clock> serializedValue =
            std::chrono::system_clock::now();
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::chrono::time_point<std::chrono::system_clock> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::vector<uint32_t>")
    {
        const std::vector<std::uint32_t> serializedValue = {89, 129, 23, 0, 12, 324};
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::vector<std::uint32_t> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::array<std::string>")
    {
        const std::array<std::string, 3> serializedValue = {"freedom ", "for ", "parrots!"};
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::array<std::string, 3> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::map<char, long int>")
    {
        const std::map<char, long int> serializedValue = {
            {'a', 1000000000}, {'b', -2000000000}, {'c', 3000000000}};
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::map<char, long int> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::list<bool>")
    {
        std::list<bool> serializedValue = {true, false, false, true, false, true, true};
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::list<bool> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::tuple<uint8_t, int, std::wstring>")
    {
        std::tuple<std::uint8_t, int, std::wstring> serializedValue =
            std::make_tuple(static_cast<std::uint8_t>(1), -12345, std::to_wstring(2374.234));
        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::tuple<std::uint8_t, int, std::wstring> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }
}

TEST_CASE("Test binary serialization & deserialization of custom types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of custom type MessageHeader")
    {
        Network::Message::MessageHeader serializedValue;
        serializedValue.mTimestamp   = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        serializedValue.mMessageType = Network::Message::MessageType::ChannelListRequest;
        serializedValue.mBodySize    = 30;
        serializedValue.mIv          = std::string("MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIB");
        serializedValue.mAuthData          = std::string("AuthData");

        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        Network::Message::MessageHeader deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue.mTimestamp == deserializedValue.mTimestamp);
        REQUIRE(serializedValue.mMessageType == deserializedValue.mMessageType);
        REQUIRE(serializedValue.mBodySize == deserializedValue.mBodySize);
        REQUIRE(serializedValue.mIv == deserializedValue.mIv);
        REQUIRE(serializedValue.mAuthData == deserializedValue.mAuthData);
    }

    SECTION("Checking serialization & deserialization of custom type ChannelInfo")
    {
        Models::ChannelInfo serializedValue(13325, 2672, "kittens");

        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        Models::ChannelInfo deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of custom type MessageInfo")
    {
        Models::MessageInfo serializedValue(3, "I'd like ice-cream=)");

        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        Models::MessageInfo deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type ChannelInfo")
    {
        std::vector<Models::ChannelInfo> serializedValue{Models::ChannelInfo(675, 2672, "kittens"),
                                                         Models::ChannelInfo(375, 1445, "puppies"),
                                                         Models::ChannelInfo(6796, 2345, "hamsters")};

        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::vector<Models::ChannelInfo> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type MessageInfo")
    {
        std::vector<Models::MessageInfo> serializedValue
        {
            Models::MessageInfo(3, "I'd like ice-cream=)"),
            Models::MessageInfo(3, "I'd like pizza=)"),
            Models::MessageInfo(3, "I'd like cheeseburger=)"),
            Models::MessageInfo(3, "I'd like naggets=)")
        };

        yas::shared_buffer buffer;
        Network::SerializedState state = Network::YasSerializer::serialize(buffer, serializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        std::vector<Models::MessageInfo> deserializedValue;
        state = Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(state == Network::SerializedState::SUCCESS);

        REQUIRE(serializedValue == deserializedValue);
    }
}
