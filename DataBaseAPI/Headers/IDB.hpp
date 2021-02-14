#pragma once

#include <string_view>

namespace DataBases
{
    template<class result>
    class IDataBaseFieldOperations
    {
        virtual result       select (const std::string_view& quary) = 0;
        virtual void         insert (const std::string_view& quary) = 0;
        virtual void         update (const std::string_view& quary) = 0;
        virtual void         del    (const std::string_view& quary) = 0;
        virtual bool         isExist(const std::string_view& quary) = 0;
    };

    class IDataBaseTableOperations
    {
        virtual void    createTable (const std::string_view& quary) = 0;
        virtual void    deleteTable (const std::string_view& quary) = 0;
    };

    class IDataBaseColumnOperations
    {
        virtual void    addNewColumn(const std::string_view& quary) = 0;
        virtual void    deleteColumn(const std::string_view& quary) = 0;
        // Changing the states of columns.
    };
}