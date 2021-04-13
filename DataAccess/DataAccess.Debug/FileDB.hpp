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

namespace DataAccess
{
    /** @class      FileDB
    * 
     *  @brief      A simple file database for debugging.
     * 
     *  @details    Each database instance is constructed with a name as constructor's argument. This name is used as a name for working directory
     *              of database (subdirectory of executable's folder). Implementation provides no check for names already in use. 
     *              Each table is stored in a subdirectory named after the table. Table directory contains "properties.json" file with 
     *              column type information, and "row_N.json" files that store actual data. Database is thread-safe,
     *              but can only serve one thread at a time.
     * 
     *  @syntax     Valid indentifier (instance/table/column name) can't start with a digit, and can only contain letters, digits 
     *              and underscores (as in Postgre, but without '$')
     *              Suppored types are : int, unsigned, bool, float and string. All numeric types are internally 64-bit by default.
     */
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

    void remove(const std::string& tableName, 
        std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; });

    bool exists(const std::string& tableName, const std::string& columnData,
                const std::string& columnName) const;

    // Column operations
    void addColumn(const std::string& tableName, const std::string& columnName,
                   const std::string& columnType);
    void removeColumn(const std::string& tableName, const std::string columnName);

    // Table operations
    void createTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                     const std::vector<std::string>& columnTypes);
    void removeTable(const std::string& tableName);

private:
    std::string name;
    std::filesystem::path path;

    mutable std::mutex mutex;

    std::map<std::string, nlohmann::ordered_json> tableProperties;
    std::map<std::string, nlohmann::ordered_json> tableRowTemplates;

    void renumerateRowFiles(const std::string& tableName) const;
};
}
