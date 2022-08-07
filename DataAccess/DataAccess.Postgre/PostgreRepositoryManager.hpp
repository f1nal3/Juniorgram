#pragma once
#include "DataAccess.Postgre/PostgreRepositoryContainer.hpp"

#include <DataAccess/IRepositoryManager.hpp>

namespace DataAccess
{
/**
 * @class   PostgreRepositoryManager
 * @brief   PostgreRepositoryManager controls handler for repository requests.
 * @details Controls push to queue and further processing of requests (have own thread for it).
 */
class PostgreRepositoryManager: public IRepositoryManager<PostgreRepositoryContainer>
{
public:
	explicit PostgreRepositoryManager(const std::shared_ptr<IAdapter>& repositoryContainer = PostgreAdapter::getInstance<PostgreAdapter>())
		: IRepositoryManager(repositoryContainer) {}
};
}  /// namespace DataAccess 
