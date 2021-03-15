#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Utility.Static/Serialize.hpp"

TEST_CASE("Test serialize object(open file text.json)", "[Serialize]")
{
    SECTION("Create text.json")
    {
        Serialize w;
    }
    SECTION("Check text.json status")
    {
        QFile jsonFile("text.json");
        if(jsonFile.open(QIODevice::ReadOnly))
        {
           REQUIRE(jsonFile.isOpen() == true);
         }
        else
        {
           REQUIRE(jsonFile.isOpen() == false);
        }
    }
}


TEST_CASE("Test serialize method updateSerialize", "[SerializeUpdateSerialize]")
{
    SECTION("Check updateSerialize")
    {
        Serialize a;
        QStringList someList;
        a.updateSerialize(someList);
        REQUIRE(someList.isEmpty() == true);
    }
}

TEST_CASE("Test serialize method pushData", "[SerializePushData]")
{
        SECTION("Create some Serialize object and add test message to text.json")
        {
            Serialize b;
            QStringList someList;
            someList.append("Test");
            b.pushData(someList);
        }
        SECTION("Check test message")
        {
            Serialize c;
            QStringList someList;
            c.updateSerialize(someList);
            REQUIRE(someList.back() == "Test");
        }    
        SECTION("Remove text.json")
        {
            QFile jsonFile("text.json");
            jsonFile.remove();
        }
}
