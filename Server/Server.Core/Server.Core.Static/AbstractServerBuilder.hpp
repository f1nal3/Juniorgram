#pragma once

namespace Server::Builder
{

class IServerBuilder
{
public:
    IServerBuilder() = default;
    virtual ~IServerBuilder() {}

    virtual void GetConnectionOptions(std::string options) = 0;
};

}  /// namespace Server::Builder
