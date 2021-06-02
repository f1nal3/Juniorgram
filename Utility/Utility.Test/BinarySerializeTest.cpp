#pragma once
#include <catch2/catch.hpp>
#include <array>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "Network/Connection.hpp"
#include "Network/Message.hpp"
#include "Network/Primitives.hpp"
#include "Utility/YasSerializer.hpp"

TEST_CASE("Test binary serialization & deserialization of plain types", "[YasSerializer]")
{
    SECTION("Checking serialization & deserialization of int type")
    {
        const int serializedValue = 10;
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        int deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of double type")
    {
        const double serializedValue = 10.9100;
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        double deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

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
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::string deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::chrono::time_point")
    {
        const std::chrono::time_point<std::chrono::system_clock> serializedValue =
            std::chrono::system_clock::now();
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::chrono::time_point<std::chrono::system_clock> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::vector<uint32_t>")
    {
        const std::vector<uint32_t> serializedValue = {89, 129, 23, 0, 12, 324};
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::vector<uint32_t> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::array<std::string>")
    {
        const std::array<std::string, 3> serializedValue = {"freedom ", "for ", "parrots!"};
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::array<std::string, 3> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::map<char, long int>")
    {
        const std::map<char, long int> serializedValue = {
            {'a', 1000000000}, {'b', -2000000000}, {'c', 3000000000}};
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::map<char, long int> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of std::list<bool>")
    {
        std::list<bool> serializedValue = {true, false, false, true, false, true, true};
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::list<bool> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }
    
    SECTION("Checking serialization & deserialization of std::tuple<uint8_t, int, std::wstring>")
    {
        std::tuple<uint8_t, int, std::wstring> serializedValue = std::make_tuple(1, -12345, std::to_wstring(2374.234));
        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::tuple<uint8_t, int, std::wstring> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }
}

TEST_CASE("Test binary serialization & deserialization of custom types", "[YasSerializer]") 
{
    SECTION("Checking serialization & deserialization of custom type MessageHeader")
    {
        Network::Message::MessageHeader serializedValue;
        serializedValue.mTimestamp   = std::chrono::system_clock::now();
        serializedValue.mMessageType = Network::Message::MessageType::ChannelListRequest;
        serializedValue.mBodySize    = 30;

        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        Network::Message::MessageHeader deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);
        REQUIRE(serializedValue.mTimestamp == deserializedValue.mTimestamp);
        REQUIRE(serializedValue.mMessageType == deserializedValue.mMessageType);
        REQUIRE(serializedValue.mBodySize == deserializedValue.mBodySize);
    }

    SECTION("Checking serialization & deserialization of custom type ChannelInfo")
    {
        Network::ChannelInfo serializedValue(2672, "kittens");

        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        Network::ChannelInfo deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of custom type MessageInfo")
    {
        Network::MessageInfo serializedValue(123, "I'd like ice-cream=)");

        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        Network::MessageInfo deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type ChannelInfo")
    {
        std::vector<Network::ChannelInfo> serializedValue{Network::ChannelInfo(2672, "kittens"),
                                                          Network::ChannelInfo(1445, "puppies"),
                                                          Network::ChannelInfo(2345, "hamsters")};

        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::vector<Network::ChannelInfo> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }

    SECTION("Checking serialization & deserialization of vector of custom type MessageInfo")
    {
        std::vector<Network::MessageInfo> serializedValue{
            Network::MessageInfo(123, "I'd like ice-cream=)"),
            Network::MessageInfo(123, "I'd like pizza=)"),
            Network::MessageInfo(123, "I'd like cheeseburger=)"),
            Network::MessageInfo(123, "I'd like naggets=)")};

        yas::shared_buffer buffer;
        Network::YasSerializer::serialize(buffer, serializedValue);

        std::vector<Network::MessageInfo> deserializedValue;
        Network::YasSerializer::deserialize(buffer, deserializedValue);

        REQUIRE(serializedValue == deserializedValue);
    }
}
