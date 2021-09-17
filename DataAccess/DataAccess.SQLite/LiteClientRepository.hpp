#pragma once
#include "LiteAdapter.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class LiteServerRepository : public AbstarctRepositoryContainer {};

	LiteServerRepository::_adapter = LiteAdapter::getInstance<LiteAdapter>();
}
