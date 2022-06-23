#pragma once

#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

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
    /**
     * @brief   FileDB constructor
     * @details Constructor looks for "instanceName" folder and loads existing tables, creates new folder otherwise
     * @param   Database instance name as string
     */
    FileDB(const std::string& instanceName);
    /**
     * @brief   FileDB destructor
     * @details Removes database folder if database has no tables
     */
    ~FileDB();

    /**
     * Prohibit copy operations 
     */
    FileDB(const FileDB&) = delete;
    FileDB& operator=(const FileDB&) = delete;

    /**
     * @brief   Database cleanup method
     * @details Removes all tables from database
     */
    void dropAllTables();
    /**
     * @brief   Method for inserting new rows into tables
     * @details Inserts "columnData" into "columnNames" of "tableName". If "columnNames" were not provided,
     *          method tries to insert data in order of columns creation (oldest to newest)
     * @param   Table name as "tableName" string, column data as vector of strings "columnData", 
     *          column names as vector of strings "columnNames"
     */
    void insert(const std::string& tableName, const std::vector<std::string>& columnData,
                const std::vector<std::string>& columnNames = {});
    /**
     * @brief   Method for retrieving data that satisfies condition from a table
     * @details Applies "condition" to all rows in "tableName" and returns rows for which "condition" returns true.
     *          Default argument will result in returning all rows
     * @param  Table name as "tableName" string, condition as function with bool(const nlohmann::json&) signature
     */
    std::vector<nlohmann::ordered_json> select(const std::string& tableName,
        std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; }) const;
    /**
     * @brief   Method for updating rows that satisfy specified condition
     * @details Applies "condition" to all rows in "tableName" and sets data to "columnData" in "columnNames"
     *          if "condition" returns true
     * @param  Table name as "tableName" string, column data as vector of strings "columnData", 
     *          column names as vector of strings "columnNames", condition as function with bool(const nlohmann::json&) signature
     */
    void update(const std::string& tableName, const std::vector<std::string>& columnData,
                const std::vector<std::string>& columnNames,
                std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; });
    /**
     * @brief   Method for removing rows that satisfy specified condition
     * @details Applies "condition" to all rows in "tableName" and removes row for which "condition" returns true.
     *          Renumerates row files afterwards.
     * @param  Table name as "tableName" string, condition as function with bool(const nlohmann::json&) signature
     */
    void remove(const std::string& tableName, 
        std::function<bool(const nlohmann::ordered_json&)> condition = [](const nlohmann::ordered_json&){ return true; });
    /**
     * @brief   Method for adding new columns to a table
     * @details Creates new column with "columnName" name and "columnType" type
     * @param  Table name as "tableName" string, column name as "columnName" string", column type as "columnType"
     */
    void addColumn(const std::string& tableName, const std::string& columnName,
                   const std::string& columnType);
    /**
     * @brief   Method for removing columns from a table
     * @details Removes column with "columnName" name from specified table
     * @param  Table name as "tableName" string, column name as "columnName" string"
     */
    void removeColumn(const std::string& tableName, const std::string columnName);
    /**
     * @brief   Method for creating new tables
     * @details Creates a new table with a name specified in "tableName", column names specified in "columnNames",
     *          column types specified in "columntypes"
     * @param  Table name as "tableName" string, column names as vector of strings "columnNames",
     *          column types as vector of strings "columnTypes"
     */
    void createTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                     const std::vector<std::string>& columnTypes);
    /**
     * @brief   Method for removing tables
     * @details Removes table with a name specified in "tableName"
     * @param  Table name as string
     */
    void removeTable(const std::string& tableName);

private:
    /**
     * Name of the database 
     */
    std::string _name;

    /**
     * Path to folder containing tables
     */
    std::filesystem::path _path;

    /**
     * Mutex used to protect class resources (both files and members)
     * Needs to be mutable to use in const functions that can read files
     */
    mutable std::mutex _mutex;

    /**
     * Contains table names as keys and their layout with number of rows as values
     */
    std::map<std::string, nlohmann::ordered_json> _tableProperties;

    /**
     * Contains table names as keys and an empty JSON with fields preset to corresponding C++ types
     */
    std::map<std::string, nlohmann::ordered_json> _tableRowTemplates;

    /**
     * @brief   Method for renumerating row files
     * @details Renames row files, maintaining their order. 
     *          Unsafe to call ouside non-locking methods since it doesn't lock any mutex
     * @param  Table name as string
     */
    void renumerateRowFiles(const std::string& tableName) const;
};
}
