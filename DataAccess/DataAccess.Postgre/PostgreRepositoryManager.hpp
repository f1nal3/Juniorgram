#pragma once
#include "PostgreRepositoryContainer.hpp"

#include <DataAccess/AbstractRepositoryManager.hpp>

namespace DataAccess
{
/**
 * @class   PostgreRepositoryManager
 * @brief   The specific(Postgre) instance of IRepositoryManager
 * @details Controls push to queue and further processing of requests (have own thread for it).
 */
class PostgreRepositoryManager: public IRepositoryManager
{
public:
	explicit PostgreRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer)
		: IRepositoryManager(repositoryContainer) {}
};
}  /// namespace DataAccess 
