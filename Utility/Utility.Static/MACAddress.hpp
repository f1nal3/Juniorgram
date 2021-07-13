#pragma once
#include "Utility/WarningSuppression.hpp"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
 
#if defined(WIN32) || defined(UNDER_CE)
#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")
#elif defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <IOKit/network/IONetworkInterface.h>
#elif defined(LINUX) || defined(linux)
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif


namespace Utility
{
    enum class MACAdressExistance : long
    {
        exists      = 0,
        nonExistent = -1
    };

    class MACAddressUtility
    {
    public:
        static std::string GetMACAddress();
        static long MACAddressUtility::takeMAC(unsigned char* result);

    private:
    #if defined(WIN32) || defined(UNDER_CE)
        static long GetMACAddressMSW(unsigned char* result);
    #elif defined(__APPLE__)
        static long GetMACAddressMAC(unsigned char* result);
    #elif defined(LINUX) || defined(linux)
        static long GetMACAddressLinux(unsigned char* result);
    #endif
};
}  // namespace Utility