#pragma once

#include <string>

namespace Base::KeyAgreement
{
/** @class IKeyAgreement
* @brief Interface determines used key agreement protocol
* @detatils Designed for use as part of Connection
*/
class IKeyAgreement
{
public:
    /// @brief Method to calculate shared key, process it and place into Session keyHolder
    virtual bool calculateSharedKey(const std::string& publicSessionServerKey, uint64_t othersideId) = 0;
    /// @brief Method for (re)generating keys based on which will be created shared(session) key
    virtual void generateKeys() = 0;
};
};  // namespace Base::KeyAgreement
