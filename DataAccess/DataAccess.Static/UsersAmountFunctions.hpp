#include "PostgreTable.hpp"
#include <Network/Primitives.hpp>

std::uint16_t findUsersAmountWithSameTableAttribute(const std::string& condition);
std::uint16_t findUsersAmountWithSameLogin(const std::string& login);
std::uint16_t findUsersAmountWithSameEmail(const std::string& email);
std::uint16_t findUsersAmountWithAllSameData(const Network::RegistrationInfo& ri);
