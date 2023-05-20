#pragma once

#include <Models/Models.hpp>

#include "PGModelFiller.hpp"

namespace DataAccess
{
	class PGChannel: public Models::Channel<PGModelFiller> 
	{
	public:
		PGChannel(const std::vector<std::pair<Models::ChannelData, std::string>>& insertData): Channel(insertData) {}
		PGChannel() = default;
	};

	class PGUser : public Models::User<PGModelFiller>
	{
	public:
		PGUser(const std::vector<std::pair<Models::UserInfo, std::string>>& insertData): User(insertData) {}
		PGUser() = default;
	};
}