#include "IConnectionVerifier.hpp"
#include "Hashing.hpp"

namespace Base::Verifiers
{
    using Base::Hashing::SHA_256;

/** @class HashVerifier
* @brief Connection verification algorithm due to
* https://github.com/f1nal3/Juniorgram/blob/dev/Docs/Towards%20Securing%20Client-Server%20Connections%20against.pdf
* @details Uses user's password hash; server's connection ID and it's public key.
*/
class HashVerifier final : public IConnectionVerifier
{
    std::string calculateVerifyingHash(const std::string& pwdHash, const ConnectionInfo& connInfo)
    {
        return SHA_256(pwdHash + std::to_string(connInfo._connectionID) + connInfo._publicServerKey);
    }
};
}  // namespace Base::Verifiers
