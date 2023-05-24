#pragma once

#include <Models/Models.hpp>

#include "PGModelFiller.hpp"

namespace DataAccess
{
	class PGChannel final: public Models::Channel<PGModelFiller> 
	{
	public:
		explicit PGChannel(const std::vector<std::pair<Models::ChannelData, std::string>>& insertData): Channel(insertData) {}
		PGChannel() = default;
	};

	class PGUser final: public Models::User<PGModelFiller>
	{
	public:
		explicit PGUser(const std::vector<std::pair<Models::UserInfo, std::string>>& insertData): User(insertData) {}
		PGUser() = default;
	};
}