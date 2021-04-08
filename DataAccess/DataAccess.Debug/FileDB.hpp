#pragma once

#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#include <nlohmann/json.hpp>

/*
 *  Indentifiers should only contain letters, digits and underscores, and can't start with a digit 
 *  Supported types : int, unsigned, bool, float and string 
 */

// TODO :           
//          !!! FORMAT COMMENTS AS IN Public\\...
//          !!! Support default argument in select and delete
//          !!! mainFileDB should become a test
//          !!! rename del function 

namespace DataAccess
{
class FileDB
{
public:
    FileDB(const std::string& instanceName);

    FileDB(const FileDB&) = delete;
    FileDB& operator=(const FileDB&) = delete;

    // Field operations
    void insert(const std::string& tableName, const std::vector<std::string>& columnData,
                const std::vector<std::string>& columnNames = {});

    std::vector<nlohmann::ordered_json> select(const std::string& tableName,
        std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; }) const;

    void update(const std::string& tableName, const std::vector<std::string>& columnData,
                const std::vector<std::string>& columnNames,
                std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; });

    void del(const std::string& tableName, 
        std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; });

    bool exists(const std::string& tableName, const std::string& columnData,
                const std::string& columnName) const;

    // Column operations
    void addColumn(const std::string& tableName, const std::string& columnName,
                   const std::string& columnType);
    void deleteColumn(const std::string& tableName, const std::string columnName);

    // Table operations
    void createTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                     const std::vector<std::string>& columnTypes);
    void deleteTable(const std::string& tableName);

private:
    std::string name;
    std::filesystem::path path;

    mutable std::mutex mutex;

    std::map<std::string, nlohmann::ordered_json> tableProperties;
    std::map<std::string, nlohmann::ordered_json> tableRowTemplates;

    void renumerateRowFiles(const std::string& tableName) const;
};
}
