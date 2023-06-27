#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "Kernel.hpp"
#include "SettingsManager.hpp"

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
    explicit KernelBuilder(std::unique_ptr<SettingsManager> settingsManager) : _settingsManager(std::move(settingsManager)) {}
    
    ~KernelBuilder() = default;

    /**
    * @brief Sets up a specific RepositoryManager
    * @details Allows to set a specific RepositoryManager
    *          Note: Changes the owner of the argument resource
    * @warning For now, this method is only used for MockTest! This may be changed in the future.
    */
    KernelBuilder& setRepoManager(std::unique_ptr<DataAccess::IRepositoryManager> repoManager)
    {
        _repository = std::move(repoManager);
        return *this;
    }

    /**
    * @brief Creates a kernel according to specific settings
    * @datails Uses private method
    */
    std::unique_ptr<Kernel> makeKernel()
    {
        std::unique_ptr<Kernel> ptr(make());
        return ptr;
    }

private:
    /**
    * @brief Creates a kernel according to specific settings
    * @datails It first creates a RepositoryManager and then a kernel with it.
    */
    Kernel* make() noexcept
    {
        auto kernel = std::unique_ptr<Kernel>(new Kernel());

        if (!_repository)
        {
            _repository = std::make_unique<DataAccess::PostgreRepositoryManager>
                (DataAccess::IAdapter::getInstance<DataAccess::PostgreAdapter>(_settingsManager->getConnectionOptions()));
        }
       
        kernel->initRepository(std::move(_repository));

        auto port = _settingsManager->getKernelPort();
        kernel->initConnection(port);

        return kernel.release();
    }

    std::unique_ptr<SettingsManager>    _settingsManager;
    RepoManagerUPtr                     _repository;
};
}  /// namespace Kernel::Builder
