#pragma once
#include "PostgreRepositoryContainer.hpp"

#include <DataAccess/AbstractRepositoryManager.hpp>

namespace DataAccess
{
/**
 * @class   PostgreRepositoryManager
 * @brief   The specific instance of IRepositoryManager
 * @details Controls push to queue and further processing of requests (have own thread for it).
 */
class PostgreRepositoryManager: public IRepositoryManager<PostgreRepositoryContainer>
{
public:
	explicit PostgreRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer)
		: IRepositoryManager(repositoryContainer) {}
};
}  /// namespace DataAccess 
