#pragma once
#include "PostgreAdapter.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class PostgreRepositoryContainer : public AbstarctRepositoryContainer 
	{
	public:
		PostgreRepositoryContainer(std::shared_ptr<IAdapter> adapter) : AbstarctRepositoryContainer(adapter) {}
	};
}
