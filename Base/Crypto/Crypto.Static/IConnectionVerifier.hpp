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
    virtual std::string calculateVerifyingHash(const std::string& pwdHash, const ConnectionInfo& connInfo) = 0;
};
}  // namespace Base::Verifiers
