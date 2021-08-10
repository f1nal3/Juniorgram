#pragma once
#define ATEXIT_FIXED

#include <Network.Static/Primitives.hpp>
#include <ctime>
#include <iostream>

#include "Utility/Exception.hpp"

#include <QVariant>

#include "DataAccess/IRepository.hpp"
#include "SQLCipherTable.hpp"

namespace DataAccess
{
class SQLCipherRepository : public IRepository
{
public:
    // Needs improvements in this place. Check this on delete issue.
    SQLCipherRepository()          = default;
    // Needs improvements in this place. 

    static SQLCipherRepository& Instance()
    {
        if (!mpInstance)
        {
            if (mDestroyed)
            {
                onDeadReference();
            }
            else
            {
                create();
            }
        }
        return *mpInstance;
    }

    inline static void killSQLCipherRepository() { mpInstance->~SQLCipherRepository(); }

    virtual ~SQLCipherRepository()
    {
        // Needs improvements in this place. Check this on delete issue and ctor placement.
        //delete mpInstance;
        mpInstance = nullptr;
        mDestroyed = true;
        // Needs improvements in this place~
    }

    std::string getRefreshToken();
    void deleteRefreshToken(); 
    bool isRefreshTokenExists();
    void setRefreshToken(const std::string& refrToken);

private:

     inline static void create()
     {
        static SQLCipherRepository theInstance;
        mpInstance = &theInstance;
     }


    inline static void onDeadReference()
    {
        create();
        new (mpInstance) SQLCipherRepository;
        #ifdef ATEXIT_FIXED
        std::atexit(killSQLCipherRepository);
        #endif
        mDestroyed = false;
    }

    inline static SQLCipherRepository* mpInstance = nullptr;
    inline static bool mDestroyed                 = false;

    virtual std::vector<std::string> getAllChannelsList() override final;
    virtual std::vector<std::string> getMessageHistoryForUser(const std::uint64_t channelID) override final;
    virtual void storeMessage(const Network::MessageInfo& message, const std::uint64_t channleID) override final;

};
}  // namespace DataAccess
