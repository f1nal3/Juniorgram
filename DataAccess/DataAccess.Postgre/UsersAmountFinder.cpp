#include "UsersAmountFinder.hpp"

#include "PostgreTable.hpp"

std::uint16_t UsersAmountFinder::findUsersAmountWithSameTableAttribute(const std::string& condition) const
{
    const auto RECORDS_AMOUNT = _pTable->Select()
                                      ->columns({"COUNT(*)"})
                                      ->Where(condition)
                                      ->execute();

    return RECORDS_AMOUNT.value()[0][0].as<std::uint16_t>();
}

std::uint16_t UsersAmountFinder::findUsersAmountWithSameLogin(const std::string& login) const
{
    return findUsersAmountWithSameTableAttribute("login='" + login + "'");
}

std::uint16_t UsersAmountFinder::findUsersAmountWithSameEmail(const std::string& email) const
{
    return findUsersAmountWithSameTableAttribute("email='" + email + "'");
}

std::uint16_t UsersAmountFinder::findUsersAmountWithAllSameData(const Models::RegistrationInfo& ri) const
{
    return findUsersAmountWithSameTableAttribute("email='" + ri.email + 
                                                 "' and login='" + ri.login +
                                                 "' and password_hash='" + ri.passwordHash + 
                                                 "'");
}
