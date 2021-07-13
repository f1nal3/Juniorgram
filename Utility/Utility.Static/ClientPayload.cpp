#include "ClientPayload.hpp"

namespace Utility
{
ClientPayload::ClientPayload(const std::string& login, const std::string& psswdHash)
    : mSub{Hashing::getSHA1HashingValue(Utility::MACAddressUtility::GetMACAddress() + login + psswdHash)},
      mOS{[]() -> std::string {
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                  return "Windows";
        #elif __APPLE__
                  return "MacOS";
        #elif __linux__
                  return "Linux";
        #endif
              }()},
              mFrom{"jg.client"}
      {}

bool operator==(const ClientPayload& payload, const ClientPayload& payloadAnother)
{
    return payload.mOS == payloadAnother.mOS && payload.mSub == payloadAnother.mSub &&
            payload.mFrom == payloadAnother.mFrom;
}

}  // namespace Utility
