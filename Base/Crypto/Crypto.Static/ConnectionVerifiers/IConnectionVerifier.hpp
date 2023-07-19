#pragma once

#include <string>

#include "Models/Primitives.hpp"

namespace Base::Verifiers
{
using Models::ConnectionInfo;

/** @class IConnectionVerifier
* @brief Interface of connection verification algorithm
* @details The interface provide to change verification algorithm in runtime. The typical use is verifing client for server,
* due to https://github.com/f1nal3/Juniorgram/blob/dev/Docs/Cryptography.md.
*/
class IConnectionVerifier
{
public:
    /* @brief Method to calculate hash to verify connection
    * @param pwdHash Result of overloaded Base::Hashing::SHA256(std::string, std::string)
    * @param connInfo Struct contains information about current connection with server.
    */
    virtual std::string calculateVerifyingHash(const std::string& pwdHash, const ConnectionInfo& connInfo) = 0;
};
}  // namespace Base::Verifiers
