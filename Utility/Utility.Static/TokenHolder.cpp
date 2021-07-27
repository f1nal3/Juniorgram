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

    void TokenHolder::setRefreshToken(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo, const std::string& refrToken)
    {
        suppressWarning(4834, Init)
        std::async(std::launch::async, &DataAccess::SQLCipherRepository::setRefreshToken,
        dynamic_cast<DataAccess::SQLCipherRepository*>(SQLCipherRepo.get()), refrToken);
        restoreWarning

        mRefreshToken = refrToken;   
    }   

    void TokenHolder::setAcccessToken(const std::string& accssToken)
    {
        mAccessToken = accssToken;
    }

    std::string TokenHolder::getRefreshToken(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo)
    { 
        if (mRefreshToken.empty())
        {
            auto refreshToken = std::async(std::launch::async, &DataAccess::SQLCipherRepository::getRefreshToken,
                       dynamic_cast<DataAccess::SQLCipherRepository*>(SQLCipherRepo.get()));
         
            if (!refreshToken.get().empty())
            {
                mRefreshToken = refreshToken.get();
                return mRefreshToken;
            }
            else
            {
                std::cout << "Please go through the authentication step!" << '\n';
                return "";
            }
        }
        else
        {
            return mRefreshToken;
        }
    }

    std::string TokenHolder::getAcccessToken()
    {
        return mAccessToken; 
    }

    TokenHolder::~TokenHolder()
    {}

    bool TokenHolder::checkTokenExistance(const std::unique_ptr<DataAccess::IRepository>& SQLCipherRepo)
    {
        auto isExists = std::async(std::launch::async, &DataAccess::SQLCipherRepository::isRefreshTokenExists,
                       dynamic_cast<DataAccess::SQLCipherRepository*>(SQLCipherRepo.get()));
        
        if (isExists.get())
        {
            auto refrToken = std::async(
                std::launch::async, &DataAccess::SQLCipherRepository::getRefreshToken,
                dynamic_cast<DataAccess::SQLCipherRepository*>(SQLCipherRepo.get()));
        
            this->setRefreshToken(SQLCipherRepo, refrToken.get());

            return true;
        }
        else
        {           
           return false;
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
