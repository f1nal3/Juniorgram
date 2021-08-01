#pragma once 

#include <string>
#include <memory>
#include <future>
#include <nlohmann/json.hpp>

#include "Utility/WarningSuppression.hpp"
#include "Cryptography.hpp"

namespace DataAccess
{
class IRepository;
}

namespace Utility
{
class TokenHolder
{
public:
    using json = nlohmann::json;

    TokenHolder(const TokenHolder& other) = delete;
    TokenHolder& operator=(const TokenHolder& other) = delete;

    TokenHolder(TokenHolder&& other) = delete;
    TokenHolder& operator=(TokenHolder&& other) = delete;

    static std::shared_ptr<TokenHolder> Instance();
   
    void setRefreshToken(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo, const std::string& refrToken);

    void setAcccessToken(const std::string& accssToken);

    std::string getRefreshToken(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo);

    bool checkTokenExistance(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo);
    
    bool isExpired(const std::string& currentToken);

    std::string& getCurrentToken();

    std::string getAcccessToken();

    virtual ~TokenHolder();

protected:
    TokenHolder() = default;

private:
    inline static std::shared_ptr<TokenHolder> mInstance{};

    std::string mAccessToken{};
    std::string mRefreshToken{};
    json mJsonObj{};
};
}  // namespace Utiltiy
