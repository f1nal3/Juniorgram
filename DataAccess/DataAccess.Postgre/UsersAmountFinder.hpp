#pragma once
#include "PostgreTable.hpp"
#include <Network/Primitives.hpp>

/** @class UsersAmountFinder.
 *   @brief UsersAmountFinder class.
 *   @details This class lets us find users amount in DataBase by some conditions.
 */
class UsersAmountFinder
{
protected:
    using Table = DataAccess::PostgreTable;

    std::unique_ptr<Table> pTable;

    UsersAmountFinder(Table* pt) : pTable(pt) {}

public:
    UsersAmountFinder() : pTable(new Table{"users"}) {}

    virtual ~UsersAmountFinder() = default;

    /** @brief Common method which let us pull from DB users amount by some condition.
     *   @params condition - SQL condition.
     *   @return users amount with the same table attribute.
     */
    std::uint16_t findUsersAmountWithSameTableAttribute(const std::string& condition) const;

    /** @brief Find users amount in DB by login.
     *   @params std::string& - user's login.
     *   @return users amount with the same login.
     */
    std::uint16_t findUsersAmountWithSameLogin(const std::string& login) const;

    /** @brief Find users amount in DB by email.
     *   @params std::string& - user's email.
     *   @return users amount with the same email.
     */
    std::uint16_t findUsersAmountWithSameEmail(const std::string& email) const;

    /** @brief Find all the same users.
     *   @params Network::RegistrationInfo& - users info.
     *   @return users amount with the same users data.
     */
    std::uint16_t findUsersAmountWithAllSameData(const Network::RegistrationInfo& ri) const;
};