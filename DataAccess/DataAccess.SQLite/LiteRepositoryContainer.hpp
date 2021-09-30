#pragma once
#include "LiteAdapter.hpp"
#include <DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class LiteRepositoryContainer : public AbstarctRepositoryContainer
	{
	public:
		LiteRepositoryContainer(std::shared_ptr<IAdapter> adapter) : AbstarctRepositoryContainer(adapter) {}
	};
}
