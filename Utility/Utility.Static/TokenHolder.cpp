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
        suppressWarning(4834, Init)
        std::async(std::launch::async, &DataAccess::SQLCipherRepository::setRefreshToken,
        /* std::reference_wrapper(*/DataAccess::SQLCipherRepository::Instance()/*)*/, refrToken);
        restoreWarning

        mRefreshToken = refrToken;   
    }   

    void TokenHolder::setAcccessToken(const std::string& accssToken)
    {
        mAccessToken = accssToken;
    }

    std::string TokenHolder::getRefreshToken()
    { 
        if (mRefreshToken.empty())
        {
            auto refreshToken = std::async(std::launch::async, &DataAccess::SQLCipherRepository::getRefreshToken,
                           &DataAccess::SQLCipherRepository::Instance());
         
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

    bool TokenHolder::checkRefrTokenExistance()
    {
        auto isExists = std::async(std::launch::async, &DataAccess::SQLCipherRepository::isRefreshTokenExists,
                       /*std::reference_wrapper(*/DataAccess::SQLCipherRepository::Instance())/*)*/;
        
        if (isExists.get())
        {
            auto refrToken = std::async(std::launch::async, &DataAccess::SQLCipherRepository::getRefreshToken,
                       /*std::reference_wrapper(*/DataAccess::SQLCipherRepository::Instance())/*)*/;
        
            this->setRefreshToken(refrToken.get());

            return true;
        }
        else
        {           
           return false;
        }
    }

    void TokenHolder::clearTokens()
    {
        mAccessToken.clear();
        mRefreshToken.clear();

        std::async(std::launch::async, &DataAccess::SQLCipherRepository::deleteRefreshToken, /*std::reference_wrapper(*/DataAccess::SQLCipherRepository::Instance())/*)*/.get();
    }

    bool TokenHolder::isExpired(const std::string& currentToken) 
    {    
        using std::chrono::system_clock;

        suppressWarning(4239, Init)
        json mJRepresentation = json::parse(Coding::getBASE64DecodedValue(Utility::extractPayload(currentToken)));
        restoreWarning

        time_t ttTimestamp = system_clock::to_time_t(system_clock::now() + std::chrono::duration_cast<system_clock::duration>(system_clock::time_point(std::chrono::seconds( std::stoll(std::string(mJRepresentation["exp"])))) - system_clock::now()));
        
        if (std::difftime(ttTimestamp, std::time(nullptr)) > 0) 
        {
            return false;
        }
        return true;
    }

    std::string& TokenHolder::getCurrentToken()
    { 
        if (!mAccessToken.empty() && !isExpired(mAccessToken))
        {
            return mAccessToken;
        }
        else if (!mRefreshToken.empty() && !isExpired(mRefreshToken))
        {
            return mRefreshToken;
        }
        else
        {
           mRefreshToken = getRefreshToken();
           return mRefreshToken; 
        }
    }
}
