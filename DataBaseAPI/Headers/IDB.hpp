#pragma once

#include <string_view>

namespace DataBases
{
    class IDataBaseFieldOperations 
    {
    public:

        //virtual result       select (const std::string_view& quary) const = 0;
        virtual void         insert (const std::string_view& tableName, 
                                     const std::string& data,
                                     const std::string_view& columnsNames = {},
                                     const std::string_view& additional   = {}) const = 0;
        virtual void         update (const std::string_view& tableName,
                                     const std::string_view& columnNameAndNewData,
                                     const std::string_view& additional = {})   const = 0;
        virtual void         del    (const std::string_view& tableName,
                                     const std::string_view& additional = {})   const = 0;
        virtual bool         isExist(const std::string_view& tableName, 
                                     const std::string_view& column,
                                     const std::string& data)              const = 0;
    };

    class IDataBaseTableOperations
    {
    public:

        virtual void    createTable (const std::string_view& tableName,
                                     const std::string_view& tableField)        const = 0;
        virtual void    deleteTable (const std::string_view& tableName)         const = 0;
    };

    class IDataBaseColumnOperations 
    {
    public:

        virtual void    addNewColumn(const std::string_view& tableName,
                                     const std::string_view& newColumn,
                                     const std::string_view& columnType)       const = 0;
        virtual void    deleteColumn(const std::string_view& tableName,
                                     const std::string_view& columnName)       const = 0;
        // Changing the states of columns.
    };
}