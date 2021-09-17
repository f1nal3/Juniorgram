#pragma once
#include "LiteAdapter.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>
#include <Utility/WarningSuppression.hpp>

namespace DataAccess
{
	class LiteServerRepository : public AbstarctRepositoryContainer {};

	suppressWarning(0, "-fpermissive")
	std::shared_ptr<IAdapter> LiteServerRepository::_adapter = LiteAdapter::getInstance<LiteAdapter>();
	std::map<LiteServerRepository::RealType, LiteServerRepository::IType> LiteServerRepository::_container = {};
	restoreWarning
}
