#include <catch2/catch.hpp>

#include "Utility.Static/JsonSerializer.hpp"

TEST_CASE("Test json serialize object(open file text.json)", "[JsonSerializer]")
{
    SECTION("Create text.json")
    {
        JsonSerializer w;
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
    SECTION("Remove text.json")
    {
       QFile jsonFile("text.json");
       jsonFile.remove();
    }
}


TEST_CASE("Test jsonserialize method updateSerialize", "[SerializeUpdateSerialize]")
{
    SECTION("Check updateSerialize")
    {
        JsonSerializer a;
        QStringList someList;
        a.updateSerialize(someList);
        REQUIRE(someList.isEmpty() == true);
    }
    SECTION("Remove text.json")
    {
        QFile jsonFile("text.json");
        jsonFile.remove();
    }

}

TEST_CASE("Test json serialize method pushData", "[SerializePushData]")
{
        SECTION("Create some JsonSerializer object and add test message to text.json")
        {
            JsonSerializer b;
            QStringList someList;
            someList.append("Test1");
            b.pushData(someList);
            someList.append("Test2");
            b.pushData(someList);
            someList.append("Test3");
            b.pushData(someList);
        }
        SECTION("Check test message")
        {
            JsonSerializer c;
            QStringList someList;
            c.updateSerialize(someList);
            if(someList.size() == 3)
            {
                REQUIRE(someList.front() == "Test1");
                REQUIRE(someList.back() == "Test3");
            }
            else
            {
                REQUIRE(false);
            }
        }    
        SECTION("Remove text.json")
        {
            QFile jsonFile("text.json");
            jsonFile.remove();
        }
}
