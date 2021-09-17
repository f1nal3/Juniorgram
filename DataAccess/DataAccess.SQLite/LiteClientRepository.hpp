#pragma once
#include "LiteAdapter.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class LiteServerRepository : public AbstarctRepositoryContainer {};

	std::shared_ptr<IAdapter> LiteServerRepository::_adapter = LiteAdapter::getInstance<LiteAdapter>();
	std::map<LiteServerRepository::RealType, LiteServerRepository::IType> LiteServerRepository::_container = {};
}
