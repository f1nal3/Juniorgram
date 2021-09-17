#pragma once
#include "PostgreAdapter.hpp"
#include <Public/Include/DataAccess/AbstarctRepositoryContainer.hpp>

namespace DataAccess
{
	class PostgreServerRepository : public AbstarctRepositoryContainer {};

	std::shared_ptr<IAdapter> PostgreServerRepository::_adapter = PostgreAdapter::getInstance<PostgreAdapter>();
	std::map<PostgreServerRepository::RealType, PostgreServerRepository::IType> PostgreServerRepository::_container = {};
}
