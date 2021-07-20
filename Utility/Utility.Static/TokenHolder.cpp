#include <DataAccess.Static/SQLCipherRepository.cpp>
#include "TokenHolder.hpp"


namespace Utility
{
    std::shared_ptr<TokenHolder> TokenHolder::Instance()
    {
        if (mInstance == nullptr)
        {
            mInstance = std::shared_ptr<TokenHolder>(new TokenHolder());
        }

        return mInstance;
    }

    void TokenHolder::setRefreshToken(const std::string& refrToken)
    {
        mRefreshToken = refrToken;
    }   

    void TokenHolder::setAcccessToken(const std::string& accssToken)
    {
        mAccessToken = accssToken;
    }

    void TokenHolder::getRefreshToken()
    {
    }

    std::string TokenHolder::getAcccessToken()
    { return mAccessToken; }

    TokenHolder::~TokenHolder()
    {}

    bool TokenHolder::checkTokenExistance(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo)
    {
        auto isExists = std::async(std::launch::async, &DataAccess::SQLCipherRepository::getRefreshToken,
                       dynamic_cast<DataAccess::SQLCipherRepository*>(SQLCipherRepo.get()));
        
        if (isExists.get().empty())
        {
            return false;
        }
        else
        {
            this->setRefreshToken(isExists.get());
            return true;
        }
    }

    bool TokenHolder::isExpired(const std::string& currentToken) 
    { 
        currentToken;
        return false;
    }
    std::string TokenHolder::extractPayload(const std::string& currentToken)
    { 
        const std::string payloadRegex = R"(.(\w|\W)*.)";
        std::regex regex(payloadRegex);
        std::smatch regex_iterator;
        if (std::regex_search(currentToken, regex_iterator, regex))
        {
            return regex_iterator.str();
        }
        return std::string("");
    }

    std::string& TokenHolder::getCurrentToken()
    { 
        if (!mAccessToken.empty())
        {
            isExpired(mAccessToken);
        }
        else
        {
        
        }

        return mAccessToken;
    }

   
}
