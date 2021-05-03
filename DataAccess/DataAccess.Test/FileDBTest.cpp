#include <catch2/catch.hpp>

#include "DataAccess.Debug/FileDB.hpp"

TEST_CASE("Database constructor")
{
	SECTION("Constructor argument") 
	{
		REQUIRE_THROWS(DataAccess::FileDB{".."});
		REQUIRE_THROWS(DataAccess::FileDB{":%?*:%"});
		REQUIRE_THROWS(DataAccess::FileDB{"0abc"});

		REQUIRE_NOTHROW(DataAccess::FileDB{"_abc"});
		REQUIRE_NOTHROW(DataAccess::FileDB{"abc____"});
		REQUIRE_NOTHROW(DataAccess::FileDB{"_24354235"});
	}

	SECTION("Read existing tables")
	{
		std::string rawPropertiesJSON = 
		{																				
                "{\n"
                "    \"column_info\" : {\"id\": \"int\",\n \"name\": \"string\", \"age\": \"float\"},\n"
                "    \"number_of_rows\" : 0\n"
                "}"
		};

		std::string rawRowJSON = 
		{																				
                "{					 \n"
				"	\"id\": 1,\n"
				"	\"name\": \"user\",\n"
				"	\"age\": 123.4\n"
				"}"
		};

		auto testPath = std::filesystem::current_path() / "test_filedb";
        std::filesystem::create_directory(testPath);
		std::filesystem::create_directory(testPath / "users");

		std::fstream fileStream;
        fileStream.open(testPath / "users" / "properties.json", std::ios::out);
		fileStream << rawPropertiesJSON;
        fileStream.close();

        fileStream.open(testPath / "users" / "row_0.json", std::ios::out);
        fileStream << rawRowJSON;
        fileStream.close();

		auto database = std::shared_ptr<DataAccess::FileDB>{new DataAccess::FileDB("test_filedb")};
		
		auto condition = [](const nlohmann::ordered_json& j) 
		{
			return (j.at("id") == 1) && (j.at("name") == "user") && (j.at("age") == 123.4);
		};

		REQUIRE(database->select("users", condition).size() == 1);

		database->dropAllTables();
	}
}

TEST_CASE("Database destructor")
{
    {
        auto database = std::shared_ptr<DataAccess::FileDB>{new DataAccess::FileDB("test_filedb3425")};
    }

	REQUIRE(std::filesystem::exists(std::filesystem::current_path() / "test_filedb3425") == false);
}

TEST_CASE("Table operations")
{
    auto database = std::shared_ptr<DataAccess::FileDB>{new DataAccess::FileDB("test_filedb")};
	
	SECTION("Table creation")
	{
        REQUIRE_THROWS(database->createTable("..", {"col"}, {"int"}));
		REQUIRE_THROWS(database->createTable("_abc", {"234"}, {"int"}));
        REQUIRE_THROWS(database->createTable("_abc", {"col"}, {"badtype"}));
        REQUIRE_THROWS(database->createTable("_abc", {"col"}, {"int", "float"}));
		REQUIRE_THROWS(database->createTable("_abc", {"col", "col2"}, {"float"}));

		REQUIRE_NOTHROW(database->createTable("users", {"id", "name", "age"}, {"int", "string", "float"}));

		nlohmann::ordered_json resultJSON;
		resultJSON["column_info"]["id"] = "int";
		resultJSON["column_info"]["name"] = "string";
		resultJSON["column_info"]["age"] = "float";
        resultJSON["number_of_rows"] = 0U;

		std::filesystem::path tablePath = std::filesystem::current_path() / "test_filedb" / "users" / "properties.json";

		REQUIRE(std::filesystem::exists(tablePath));

		nlohmann::ordered_json generatedJSON;

		std::fstream fileStream;
		fileStream.open(tablePath, std::ios::in);
		generatedJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE(generatedJSON == resultJSON);
	}

	SECTION("Table removal")
	{
		REQUIRE_THROWS(database->removeTable("f329hw9hrg932"));

		database->removeTable("users");

		REQUIRE(std::filesystem::exists(std::filesystem::current_path() / "test_db" / "users") == false);
	}
}

TEST_CASE("Column operations")
{
    auto database = std::shared_ptr<DataAccess::FileDB>{new DataAccess::FileDB("test_filedb")};
    std::filesystem::path testPath{std::filesystem::current_path() / "test_filedb" / "users"};

	SECTION("Column creation")
	{
		REQUIRE_THROWS(database->addColumn("badtable", "name", "int"));

		database->createTable("users", {}, {});

		REQUIRE_THROWS(database->addColumn("users", "1abc", "int"));
		REQUIRE_THROWS(database->addColumn("users", "..", "int"));
		REQUIRE_THROWS(database->addColumn("users", "_abc", "badtype"));

		REQUIRE_NOTHROW(database->addColumn("users", "id", "int"));

		nlohmann::ordered_json propertiesJSON;
		nlohmann::ordered_json rowJSON;

		std::fstream fileStream;
		fileStream.open(testPath / "properties.json", std::ios::in);
		propertiesJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE(propertiesJSON["column_info"]["id"] == "int");

		REQUIRE_NOTHROW(database->insert("users", {"2"}, {"id"}));

		fileStream.open(testPath / "row_0.json", std::ios::in);
		rowJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE(rowJSON["id"] == 2);
	}

	SECTION("Column removal")
	{
		database->insert("users", {"1"}, {"id"});
		
		REQUIRE_THROWS(database->removeColumn("badtable", "id"));
		REQUIRE_NOTHROW(database->removeColumn("users", "badcolumn"));
		REQUIRE_NOTHROW(database->removeColumn("users", "id"));

		std::fstream fileStream;
		fileStream.open(testPath / "properties.json", std::ios::in);
		nlohmann::ordered_json propertiesJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE_THROWS(propertiesJSON.at("id"));

		fileStream.open(testPath / "row_0.json", std::ios::in);
		nlohmann::ordered_json rowJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE_THROWS(rowJSON.at("id"));

		database->dropAllTables();
	}
}

TEST_CASE("Row operations")
{
    auto database = std::shared_ptr<DataAccess::FileDB>{new DataAccess::FileDB("test_filedb")};
    std::filesystem::path testPath{std::filesystem::current_path() / "test_filedb" / "users"};

	SECTION("Insert")
	{
		database->createTable("users", {}, {});
        database->addColumn("users", "id", "int");

		REQUIRE_THROWS(database->insert("badtable", {"1"}, {"id"}));
        REQUIRE_THROWS(database->insert("users", {"1"}, {"badcolumn"}));
		REQUIRE_THROWS(database->insert("users", {"badtype"}, {"id"}));
        REQUIRE_THROWS(database->insert("users", {"badtype"}));
		REQUIRE_THROWS(database->insert("users", {"1", "2"}));
        REQUIRE_THROWS(database->insert("users", {}, {"id"}));

		REQUIRE_NOTHROW(database->insert("users", { "1" }, {"id"}));
		REQUIRE_NOTHROW(database->insert("users", {"2"}));

		std::fstream fileStream;
		fileStream.open(testPath / "properties.json", std::ios::in);
		nlohmann::ordered_json propertiesJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		fileStream.close();

		fileStream.open(testPath / "row_0.json", std::ios::in);
		nlohmann::ordered_json rowJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
			std::istreambuf_iterator<char>());
		fileStream.close();

		REQUIRE(rowJSON.at("id") == 1);
		REQUIRE(propertiesJSON.at("number_of_rows") == 2U);
	}

	SECTION("Select")
	{
		REQUIRE_THROWS(database->select("badtable"));

		REQUIRE(database->select("users").size() == 2);
		REQUIRE(database->select("users", [](const nlohmann::ordered_json& j) { return j.at("id") < 0;}).size() == 0);
	}

	SECTION("Update") 
	{ 
		REQUIRE_THROWS(database->update("badtable", {"1"}, {"id"}));
        REQUIRE_THROWS(database->update("users", {"1"}, {"badcolumn"}));
		REQUIRE_THROWS(database->update("users", {"badtype"}, {"id"}));
        REQUIRE_THROWS(database->update("users", {"1", "2"}, {"id"}));

		REQUIRE_NOTHROW(database->update("users", {"0"}, {"id"}));
        REQUIRE(database->select("users", [](const nlohmann::ordered_json& j) { return j.at("id") == 0;}).size() == 2);

		REQUIRE_NOTHROW(database->update("users", {"-8"}, {"id"}),
                        [](const nlohmann::ordered_json& j) { return j.at("id") == 0; });
		REQUIRE(database->select("users", [](const nlohmann::ordered_json& j) { return j.at("id") == -8;}).size() == 2);
	}

	SECTION("Remove")
	{
		REQUIRE_THROWS(database->remove("badtable"));
		REQUIRE_NOTHROW(database->remove("users", [](const nlohmann::ordered_json& j) { return j.at("id") == -8;}));
        REQUIRE_NOTHROW(database->remove("users"));

		std::fstream fileStream;
        fileStream.open(testPath / "properties.json", std::ios::in);
        nlohmann::ordered_json propertiesJSON = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                                       std::istreambuf_iterator<char>());
        fileStream.close();

		REQUIRE(propertiesJSON.at("number_of_rows") == 0U);

		database->dropAllTables();
	}
}
