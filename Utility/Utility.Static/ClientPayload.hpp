    #pragma once

#include "Cryptography.hpp"
#include "MACAddress.hpp"
#include <string>

namespace Utility
{
struct ClientPayload
{
    ClientPayload(const std::string& login, const std::string& psswdHash);

    ClientPayload& operator=(const ClientPayload& other) = default;

    friend bool operator==(const ClientPayload& payload, const ClientPayload& payloadAnother);

private:
    std::string mOS;
    std::string mSub;
    std::string mFingerprint;
};
}  // namespace Utility