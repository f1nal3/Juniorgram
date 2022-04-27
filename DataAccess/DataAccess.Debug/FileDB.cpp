#include "FileDB.hpp"

#include <iomanip>
#include <stdexcept>

namespace
{
namespace fs = std::filesystem;


/**
 * @brief   Function that constructs empty properties JSON
 */
nlohmann::ordered_json constructPropertiesJSON() { return nlohmann::ordered_json{{"column_info", nlohmann::ordered_json()}, {"number_of_rows", 0U}}; }

/**
 * @brief   Function that checks if input identifier is valid
 * @details Valid identifier is not empty, starts with either an underscore or a character
 *          and can contain only characters, digits and underscores
 * @param  Identifier as string
 */
bool isValidIdentifier(const std::string& identifier)
{
    if (identifier.empty() || std::isdigit(identifier[0]))
    {
        return false;
    }

    for (const auto& c : identifier)
        if (!std::isalnum(c) && c != '_') return false;

    return true;
}

/**
 * @brief   Function that checks if input datatype is valid
 * @details Valid datatypes are int, unsigned, float, bool, string
 * @param  Datatype as string
 */
bool isValidDatatype(const std::string& dataType)
{
    const std::vector<std::string> validTypes = {"int", "unsigned", "float", "bool", "string"};

    for (const auto& validType : validTypes)
        if (dataType == validType) return true;

    return false;
}

/**
 * @brief   Function that sets JSON field to appropriate C++ type
 * @details If not fieldValue has been supplied, function uses 0 as default
 *          Value from fieldValue string is explicitly converted to fieldType
 * @param  Target json as nlohmann::ordered_json
 *          fieldName as string
 *          fieldType as string
 *          fieldValue as string ("0" by default)
 */
void setJSONFieldType(nlohmann::ordered_json& json, const std::string& fieldName,
                              const std::string& fieldType, const std::string& fieldValue = "0")
{
    const std::vector<std::string> validTypes = {"int", "unsigned", "float", "bool", "string"};

    auto targetTypeIndex =
        (std::find(validTypes.begin(), validTypes.end(), fieldType)) - validTypes.begin();

    switch (targetTypeIndex)
    {
        case 0:
        {
            json[fieldName] = std::stoll(fieldValue);
        }
        break;

        case 1:
        {
            json[fieldName] = std::stoull(fieldValue);
        }
        break;

        case 2:
        {
            json[fieldName] = std::stod(fieldValue);
        }
        break;

        case 3:
        {
            json[fieldName] = (fieldValue.empty() || fieldValue == "0" ? false : true);
        }
        break;

        case 4:
        {
            json[fieldName] = fieldValue;
        }
        break;

        default:
        {
            throw std::invalid_argument("Invalid type : \"" + fieldType + "\"");
        }
    }
}
}

namespace DataAccess
{

FileDB::FileDB(const std::string& instanceName)
{
    if (isValidIdentifier(instanceName))
    {
        _name = instanceName;
        _path = fs::current_path().append(instanceName);
    }
    else
    {
        throw std::invalid_argument("Invalid identifier : \"" + instanceName + "\"");
    }

    if (!fs::exists(_path))
    {
        fs::create_directory(_path);
    }
    else
    {
        std::fstream fileStream;

        for (auto& directoryEntry : fs::directory_iterator(_path))
        {
            if (directoryEntry.is_directory())
            {
                fs::path tablePath    = directoryEntry.path();
                std::string tableName = tablePath.filename().string();

                if (isValidIdentifier(tableName) &&
                    fs::exists(tablePath / "properties.json"))
                {
                    nlohmann::ordered_json properties;

                    fileStream.open(tablePath / "properties.json", std::ios::in);
                    properties = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                                       std::istreambuf_iterator<char>());
                    fileStream.close();

                    _tableProperties[tableName] = properties;

                    nlohmann::ordered_json rowTemplate;

                    for (const auto& [columnName, columnType] : properties["column_info"].items())
                    {
                        setJSONFieldType(rowTemplate, columnName, columnType);
                    }

                    _tableRowTemplates[tableName] = rowTemplate;
                }
            }
        }
    }
}

FileDB::~FileDB()
{
    if (_tableProperties.size() == 0)
    {
        fs::remove(_path);
    }
}

void FileDB::dropAllTables()
{
    auto propertiesCopy = _tableProperties;

    for (auto [tableName, properties] : propertiesCopy)
    {
        removeTable(tableName);    
    }
}

void FileDB::insert(const std::string& tableName, const std::vector<std::string>& columnData,
            const std::vector<std::string>& columnNames)
{
    std::scoped_lock<std::mutex> lock{ _mutex };

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    nlohmann::ordered_json& properties = _tableProperties[tableName];

    if ((columnNames.size() != columnData.size()) && columnNames.size() != 0)
    {
        throw std::invalid_argument("Number of columns/initializers mismatch");
    }

    nlohmann::ordered_json row = _tableRowTemplates[tableName];

    if (columnData.size() > row.size())
    {
        throw std::invalid_argument("Too many arguments : table \"" + tableName + "\" has " + std::to_string(row.size()) 
        + " columns, but " + std::to_string(columnData.size()) + " arguments have been supplied");
    }

    if (columnNames.empty())
    {
        size_t offset{ 0 };

        for (auto& [field, value] : row.items())
        {
            std::string currentData = *(columnData.begin() + (offset++));
            setJSONFieldType(row, field, properties["column_info"][field], currentData);
        }
    }
    else
    {
    for (size_t i = 0; i != columnNames.size(); ++i)
        {
            if (!isValidIdentifier(columnNames[i]))
            {
                throw std::invalid_argument("Invalid identifier : \"" + columnNames[i] + "\"");
            }

            if (row.find(columnNames[i]) != row.end())
            {
                setJSONFieldType(row, columnNames[i], properties["column_info"][columnNames[i]], columnData[i]);
            }
            else
            {
                throw std::invalid_argument("Column \"" + columnNames[i] + "\" does not exist.");
            }
        }
    }

    std::string rowFilename =
        "row_" + std::to_string(properties["number_of_rows"].get<unsigned>()) + ".json";

    properties["number_of_rows"] = properties["number_of_rows"].get<unsigned>() + 1U;

    std::fstream fileStream;

    fileStream.open(_path / tableName / "properties.json",
                    std::ios::out);
    fileStream << std::setw(4) << properties;
    fileStream.close();

    fs::path rowPath = _path / tableName / rowFilename;

    fileStream.open(rowPath, std::ios::out);
    fileStream << std::setw(4) << row;
    fileStream.close();
}

std::vector<nlohmann::ordered_json> FileDB::select(const std::string& tableName, std::function<bool(const nlohmann::ordered_json&)> condition) const
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    nlohmann::ordered_json properties = _tableProperties.at(tableName);

    fs::path tablePath = _path / tableName;
    std::vector<nlohmann::ordered_json> results;

    std::fstream fileStream;
    nlohmann::ordered_json row;
    
    for (fs::directory_entry directoryEntry : fs::directory_iterator(tablePath))
    {
        if (directoryEntry.path().filename().string().substr(0, 4) == "row_")
        {
            fileStream.open(directoryEntry.path(), std::ios::in);
            row = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                                std::istreambuf_iterator<char>());
            fileStream.close();

            if (condition(row))
            {
                results.push_back(row);
            }
        }
    }

    return results;
}

void FileDB::update(const std::string& tableName, const std::vector<std::string>& columnData,
            const std::vector<std::string>& columnNames,
            std::function<bool(const nlohmann::ordered_json&)> condition)
{
    std::scoped_lock<std::mutex> lock{ _mutex };

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    const nlohmann::ordered_json& properties = _tableProperties[tableName];

    fs::path tablePath = _path / tableName;

    for (auto columnName : columnNames)
    {
        if (properties["column_info"].find(columnName) == properties["column_info"].end())
        {
            throw std::invalid_argument("Table \"" + tableName +
                                        "\" has no column \"" + columnName + "\"");
        }
    }

    if (columnNames.size() != columnData.size())
    {
        throw std::invalid_argument("Number of columns/data mismatch");
    }

    std::fstream fileStream;
    nlohmann::ordered_json row;

    for (fs::directory_entry directoryEntry : fs::directory_iterator(tablePath))
    {
        if (directoryEntry.path().filename().string().substr(0, 4) == "row_")
        {
            fileStream.open(directoryEntry.path(), std::ios::in);
            row = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                        std::istreambuf_iterator<char>());
            fileStream.close();

            if (condition(row))
            {
                for (size_t i = 0; i != columnNames.size(); ++i)
                {
                    setJSONFieldType(row, columnNames[i], properties["column_info"][columnNames[i]], columnData[i]);
                }

                fileStream.open(directoryEntry.path(), std::ios::out);
                fileStream << std::setw(4) << row;
                fileStream.close();
            }
        }
    }
}

void FileDB::remove(const std::string& tableName,
                 std::function<bool(const nlohmann::ordered_json&)> condition)
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    nlohmann::ordered_json& properties = _tableProperties[tableName];

    fs::path tablePath = _path / tableName;

    std::fstream fileStream;
    nlohmann::ordered_json row;

    for (fs::directory_entry directoryEntry : fs::directory_iterator(tablePath))
    {
        if (directoryEntry.path().filename().string().substr(0, 4) == "row_")
        {
            fileStream.open(directoryEntry.path(), std::ios::in);
            row = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                                std::istreambuf_iterator<char>());
            fileStream.close();

            if (condition(row))
            {
                fs::remove(directoryEntry.path());
                properties["number_of_rows"] = properties["number_of_rows"].get<unsigned>() - 1U;
            }
        }
    }

    fileStream.open(_path / tableName / "properties.json", std::ios::out);
    fileStream << std::setw(4) << properties;
    fileStream.close();

    renumerateRowFiles(tableName);
}

void FileDB::createTable(const std::string& tableName, const std::vector<std::string>& columnNames,
                          const std::vector<std::string>& columnTypes)
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (!isValidIdentifier(tableName))
    {
        throw std::invalid_argument("Invalid identifier : \"" + tableName + "\"");
    }

    if (_tableProperties.find(tableName) != _tableProperties.end())
    {
        throw std::invalid_argument("Table already exists.");
    }

    fs::path tablePath = _path / tableName;


    if (columnNames.size() != columnTypes.size())
    {
        throw std::invalid_argument("Number of columns/initializers mismatch");
    }


    nlohmann::ordered_json properties = constructPropertiesJSON();

    for (size_t i = 0; i != columnNames.size(); ++i)
    {
        if (isValidIdentifier(columnNames[i]) && isValidDatatype(columnTypes[i]))
        {
            properties["column_info"][columnNames[i]] = columnTypes[i];
        }
        else
        {
            throw std::invalid_argument("Invalid identifier/type pair : \"" + columnNames[i] +
                                        ", " + columnTypes[i] + "\"");
        }
    }

    nlohmann::ordered_json rowTemplate;

    for (const auto& [columnName, columnType] : properties["column_info"].items())
    {
        setJSONFieldType(rowTemplate, columnName, columnType);
    }

    _tableRowTemplates[tableName] = rowTemplate;

    if (!fs::exists(tablePath))
    {
        fs::create_directory(tablePath);
    }

    std::fstream fileStream;

    fileStream.open(tablePath / "properties.json", std::ios::out);
    fileStream << std::setw(4) << properties;
    fileStream.close();

    _tableProperties[tableName] = properties;
}

void FileDB::removeTable(const std::string& tableName)
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    _tableProperties.erase(tableName);

    fs::path tablePath = _path / tableName;

    if (fs::exists(tablePath))
    {
        fs::remove_all(tablePath);
    }
}

void FileDB::addColumn(const std::string& tableName, const std::string& columnName,
               const std::string& columnType)
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (!isValidIdentifier(columnName) || !isValidDatatype(columnType))
    {
        throw std::invalid_argument("Invalid identifier/type pair : \"" + columnName +
                                    " : " + columnType + "\"");
    }

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    nlohmann::ordered_json& properties = _tableProperties[tableName];

    fs::path tablePath = _path / tableName;

    std::fstream fileStream;

    if (properties["column_info"].find(columnName) != properties["column_info"].end())
    {
        throw std::invalid_argument("Column \"" + columnName + "\" already exists.");
    }

    properties["column_info"][columnName] = columnType;

    fileStream.open(tablePath / "properties.json", std::ios::out);
    fileStream << std::setw(4) << properties;
    fileStream.close();

    setJSONFieldType(_tableRowTemplates[tableName], columnName, columnType);
}

void FileDB::removeColumn(const std::string& tableName, const std::string columnName)
{
    std::scoped_lock<std::mutex> lock{_mutex};

    if (_tableProperties.find(tableName) == _tableProperties.end())
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    nlohmann::ordered_json& properties = _tableProperties[tableName];

    fs::path tablePath = _path / tableName;

    std::fstream fileStream;

    if (properties["column_info"].find(columnName) != properties["column_info"].end())
    {
        properties["column_info"].erase(columnName);

        fileStream.open(tablePath / "properties.json", std::ios::out);
        fileStream << std::setw(4) << properties;
        fileStream.close();
    }

    for (fs::directory_entry directoryEntry : fs::directory_iterator(tablePath))
    {
        if (directoryEntry.path().filename().string().substr(0, 4) == "row_")
        {
            fileStream.open(directoryEntry.path(), std::ios::in);
            nlohmann::ordered_json row = nlohmann::ordered_json::parse((std::istreambuf_iterator<char>(fileStream)),
                                                std::istreambuf_iterator<char>());
            fileStream.close();

            row.erase(columnName);

            fileStream.open(directoryEntry.path(), std::ios::out);
            fileStream << row;
            fileStream.close();
        }
    }

    _tableRowTemplates[tableName].erase(columnName);
}

void FileDB::renumerateRowFiles(const std::string& tableName) const
{
    fs::path tablePath;

    if (_tableProperties.find(tableName) != _tableProperties.end())
    {
        tablePath = _path / tableName;
    }
    else
    {
        throw std::invalid_argument("Table \"" + tableName + "\" does not exist.");
    }

    std::vector<std::string> rowFilenames;

    for (fs::directory_entry directoryEntry : fs::directory_iterator(tablePath))
    {
        if (directoryEntry.path().filename().string().substr(0, 4) == "row_")
        {
            rowFilenames.push_back(directoryEntry.path().filename().string());
        }
    }

    std::sort(rowFilenames.begin(), rowFilenames.end(), [](std::string lhs, std::string rhs) {
        lhs = lhs.substr(4, lhs.size() - 9);
        rhs = rhs.substr(4, rhs.size() - 9);

        return std::stoul(lhs) < std::stoul(rhs);
    });

    for (size_t i = 0; i != rowFilenames.size(); ++i)
    {
        std::string newFilename = "row_" + std::to_string(i) + ".json";
        fs::rename(tablePath / rowFilenames[i], tablePath / newFilename);
    }
}

} 
