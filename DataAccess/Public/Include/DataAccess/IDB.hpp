#pragma once

#include <string_view>
#include <vector>

namespace DataAccess
{

    using typeDataName = std::vector<std::pair<std::string, std::string>>;
    using columnData   = std::vector<std::pair<std::string, std::string>>;

    class IDataBaseFieldOperations 
    {
    public:

        //virtual              select (const std::string_view& quary)           const = 0;
        virtual void         insert (const std::string_view& tableName, 
                                     const columnData& columnsAndData = {},
                                     const std::string_view& additional   = {}) const = 0;
        virtual void         update (const std::string_view& tableName,
                                     const columnData& columnAndNewData,
                                     const std::string_view& additional = {})   const = 0;
        virtual void         del    (const std::string_view& tableName,
                                     const std::string_view& additional = {})   const = 0;
        virtual bool         isExist(const std::string_view& tableName, 
                                     const std::string& column,
                                     const std::string& data)                   const = 0;
    };

    class IDataBaseTableOperations
    {
    public:

        virtual void    createTable (const std::string& tableName,
                                     const typeDataName& tableField)             const = 0;
        virtual void    deleteTable (const std::string& tableName)              const = 0;
    };

    class IDataBaseColumnOperations 
    {
    public:

        virtual void    addNewColumn(const std::string_view& tableName,
                                     const std::string_view& newColumn,
                                     const std::string_view& columnType)       const = 0;
        virtual void    deleteColumn(const std::string_view& tableName,
                                     const std::string_view& columnName)       const = 0;
    };
}

