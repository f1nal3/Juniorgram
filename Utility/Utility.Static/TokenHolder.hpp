#pragma once 

#include <string>
#include <memory>
#include <future>
#include <chrono>
#include <nlohmann/json.hpp>

#include "Utility/WarningSuppression.hpp"
#include "Cryptography.hpp"

namespace DataAccess
{
class IRepository;
}

namespace Utility
{
using AccessAndRefreshToken = std::pair<std::string, std::string>;

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

    std::string getRefreshToken();

    bool checkRefrTokenExistance();
    
    void clearTokens();

    bool isExpired(const std::string& currentToken);

    std::string& getCurrentToken();

    std::string getAccessToken();

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
