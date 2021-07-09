#pragma once
#include "PostgreTable.hpp"
#include <Network/Primitives.hpp>

class UsersAmountFinder
{
protected:
    using Table = DataAccess::PostgreTable;

    std::unique_ptr<Table> pTable;

    UsersAmountFinder(Table* pt) : pTable(pt) {}

public:
    UsersAmountFinder() : pTable(new Table{"users"}) {}

    virtual ~UsersAmountFinder() = default;

    std::uint16_t UsersAmountFinder::findUsersAmountWithSameTableAttribute(
        const std::string& condition) const;

    std::uint16_t UsersAmountFinder::findUsersAmountWithSameLogin(const std::string& login) const;

    std::uint16_t UsersAmountFinder::findUsersAmountWithSameEmail(const std::string& email) const;

    std::uint16_t UsersAmountFinder::findUsersAmountWithAllSameData(
        const Network::RegistrationInfo& ri) const;
};
