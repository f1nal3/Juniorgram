#pragma once
#include "DataAccess/IClientRepositories.hpp"
#include "LiteTable.hpp"


namespace DataAccess
{
    struct AbstractLiteRepository
    {
    protected:
        std::unique_ptr<LiteTable> pTable;
    };
}
