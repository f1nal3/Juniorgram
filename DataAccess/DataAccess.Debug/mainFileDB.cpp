#include <iostream>
#include "FileDB.hpp"

int main() 
{
    try
    {
        auto database = std::shared_ptr<DataAccess::FileDB>{ new DataAccess::FileDB("test") };

        database->deleteTable("users");
        database->createTable("users", {"id", "name", "age"}, {"int", "string", "float" });

        database->addColumn("users", "test", "string");
        database->deleteColumn("users", "test");


        database->insert("users", {"1", "vavan", "21.5"}, {"id", "name", "age"});
        database->insert("users", {"2", "vavan", "21.5"}, {"id", "name", "age"});
        database->insert("users", {"3", "vavan", "21.5"}, {"id", "name", "age"});

        database->insert("users", {"4", "vavan", "20.5"});
        database->insert("users", {"5", "vavan", "20.5"});

        database->insert("users", {"6", "vavan", "22.5"}, {"id", "name", "age"});
        database->insert("users", {"7", "vavan", "22.5"}, {"id", "name", "age"});
        database->insert("users", {"8", "vavan", "22.5"}, {"id", "name", "age"});
        

        database->update("users", {"big_vavan"}, {"name"}, [](const nlohmann::ordered_json& j) { return j["age"] == 22.5; });
        database->del("users", [](const nlohmann::ordered_json& j) { return j["age"] < 21.0; });

        std::cout << std::boolalpha << database->exists("users", "big_vavan", "name") << '\n';
        std::cout << std::boolalpha << database->exists("users", "20.5", "age") << '\n';

        auto v = database->select("users", [](const nlohmann::ordered_json& j) { return j["age"] == 21.5;});
        for (auto j : v)
        {
            std::cout << j["id"] << '\n';
        }

        database->insert("users", {"8", "vavan", "22.5", "1", "2", "3", "4"});
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}   