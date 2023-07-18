#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "Server.hpp"  //TO DO: We'll have to change include filename accordenly the task of renaming files and directories

namespace Kernel::Builder
{
/**
 * @brief Class builder of Kernel.
 * @details Class based on builder pattern for creation instances of @link Kernel @endlink.
 */
class KernelBuilder 
{
public:
    KernelBuilder()  = default;
    ~KernelBuilder() = default;

    inline KernelBuilder& setValue(std::pair<std::string, std::string> keyValue)
    {
        _arguments.insert(keyValue);
        return *this;
    }

    inline KernelBuilder& setValue(DataAccess::IRepositoryManager* repoManager)
    {
        _repository.reset(repoManager);
        return *this;
    }

    std::unique_ptr<Kernel> makeKernel()
    {
        std::unique_ptr<Kernel> ptr(make());
        return ptr;
    }

private:
    Kernel* make() noexcept
    {
        // form a string with the configuration for connecting to the database
        std::string dbOptions = "";
        for (const auto& [configName, configValue] : _arguments)
        {
            if (configName != "--kernelport")
            {
                for (size_t i = 0; i < configName.length(); ++i)
                {
                    if (configName[i] != '-')
                    {
                        dbOptions += configName[i];
                    }
                }
                dbOptions += "=";
                dbOptions += configValue;
                dbOptions += " ";
            }
        }
        
        auto kernel = std::unique_ptr<Kernel>{new Kernel()};

        if (!_repository)
        {
            auto repoManager = std::make_unique<DataAccess::PostgreRepositoryManager>(
                DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(dbOptions));
            kernel->initRepository(std::move(repoManager));
        }
        else
        {
            kernel->initRepository(std::move(_repository));
        }

        auto port = static_cast<uint16_t>(std::stoi(_arguments["--kernelport"]));
        kernel->initConnection(port);

        return kernel.release();
    }

    std::map<std::string, std::string>  _arguments;
    RepoManagerUPtr                     _repository;
};
}  /// namespace Kernel::Builder
