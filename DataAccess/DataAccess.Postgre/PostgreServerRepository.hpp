#pragma once
#include "PostgreAdapter.hpp"
#include "Utility/WarningSuppression.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class PostgreServerRepository : public AbstarctRepositoryContainer {};

	suppressWarning(0, "-fpermissive")
	std::shared_ptr<IAdapter> PostgreServerRepository::_adapter = PostgreAdapter::getInstance<PostgreAdapter>();
	std::map<PostgreServerRepository::RealType, PostgreServerRepository::IType> PostgreServerRepository::_container = {};
	restoreWarning
}
