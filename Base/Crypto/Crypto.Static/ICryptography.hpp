#pragma once

#include <Network/YasSerializer.hpp>
#include <Utility/Utility.hpp>
#include <Network/Message.hpp>

namespace Base::Crypto
{
    using Network::Message;

class ICryptography
{
public:
    virtual Utility::GeneralCodes encrypt(const Message& message, yas::shared_buffer& bodyBuffer) = 0;
    virtual Utility::GeneralCodes decrypt(yas::shared_buffer& buffer, Message& message)           = 0;
    virtual ~ICryptography()                                                                      = 0 {};
};
}
