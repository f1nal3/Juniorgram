#pragma once

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <utility>

#include "IAdapter.hpp"
#include "IMasterRepository.hpp"

namespace DataAccess
{
/* @class AbstractRepositoryContainer
 * @brief An abstract class for kernel and client global repositories.
 * @details Consists of adapter to chosen database, and map of registered repositories.
 * This class provide registration of repositories, and getting them. Requirement for repository registration:
 * interface for your repository is exists (e.g. you have LoginRepository that inherit its interface - ILoginRepository,
 * so you can register LoginRepository). You can get the repository by specifying its interface.
 * (e.g. getRepository<ILoginRepository>()).
 */
class AbstractRepositoryContainer
{
public:
    using RealType = std::string;
    using IType    = std::shared_ptr<IMasterRepository>;

    explicit AbstractRepositoryContainer(std::shared_ptr<IAdapter> adapter) :
        _adapter(std::move(adapter)), _container() {}

    /**
     * @brief Method for register new repositories.
     * @warning You should pass, firstly, an interface and after an implementation of this repository
     */
    template <typename TInterface, typename TRepository>
    void registerRepository()
    {
        //TODO check that they are all child of IMasterRepository
        static_assert(std::is_base_of_v<TInterface, TRepository>, "Repository does not implement current Interface!");

        const char* typeInfo = typeid(TInterface).name();

        if (_container.find(typeInfo) != _container.end())
        {
            throw std::runtime_error(std::string("This interface has already registered! - ") + typeInfo);
        }

        _container.emplace(typeInfo, std::make_shared<TRepository>(_adapter));
    }

    /**
     * @brief Method for get a repositories.
     * @warning You should pass an interface
     */
    template <typename TInterface>
    std::shared_ptr<TInterface> getRepository()
    {
        const char* typeInfo = typeid(TInterface).name();

        if (_container.find(typeInfo) == _container.end())
        {
            throw std::runtime_error(std::string("This repository was not registered! - ") + typeInfo);
        }

        return std::dynamic_pointer_cast<TInterface>(_container[typeInfo]);
    }

private:
    std::shared_ptr<IAdapter> _adapter;
    std::map<RealType, IType> _container;
};
}  // namespace DataAccess
