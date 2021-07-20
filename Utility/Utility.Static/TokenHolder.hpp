#pragma once 

#include <string>
#include <memory>
#include <regex>
#include <future>
#include <nlohmann/json.hpp>

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
   
    void setRefreshToken(const std::string& refrToken);

    void setAcccessToken(const std::string& accssToken);

    void getRefreshToken();

    bool checkTokenExistance(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo);
    
    bool isExpired(const std::string& currentToken);

    std::string& getCurrentToken();

    std::string extractPayload(const std::string& currentToken);

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
