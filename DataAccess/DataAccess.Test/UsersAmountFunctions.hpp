#include <DataAccess.Static/PostgreTable.hpp>

std::uint16_t findUsersAmountWithSameTableAttribute(const std::string& condition)
{
    const auto RECORDS_AMOUNT = DataAccess::PostgreTable("users")
                                    .Select()
                                    ->columns({"COUNT(*)"})
                                    ->Where(condition)
                                    ->execute();

    return RECORDS_AMOUNT.value()[0][0].as<std::uint16_t>();
};

std::uint16_t findUsersAmountWithSameLogin(const std::string& login)
{
    return findUsersAmountWithSameTableAttribute("login='" + login + "'");
};

std::uint16_t findUsersAmountWithSameEmail(const std::string& email)
{
    return findUsersAmountWithSameTableAttribute("email='" + email + "'");
};

std::uint16_t findUsersAmountWithAllSameData(const Network::RegistrationInfo& ri)
{
    return findUsersAmountWithSameTableAttribute("email='" + ri.email + "' and login='" + ri.login +
                                                 "' and password_hash='" + ri.passwordHash + "'");
};
