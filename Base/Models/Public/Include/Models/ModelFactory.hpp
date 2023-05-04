#include <memory>
#include <type_traits>
#include <utility>

#include "UnifyedModel.hpp"
#include "Models.hpp"

namespace Models
{

	template<template <typename> class RepositoryType>
	class ModelFactory
	{
	public:
		ModelFactory() = default;

		~ModelFactory() = default;


	public:
		template<typename TModel>
		std::unique_ptr<RepositoryType<TModel>> create(const std::vector<std::pair<TModel, std::string>>& insertData = {})
		{			
			if (std::is_same_v<TModel, ChannelData>)
			{
				return std::unique_ptr<RepositoryType<TModel>>(new Channel<RepositoryType>(insertData));
			}
			else if (std::is_same_v<TModel, UserInfo>)
			{
				//return std::unique_ptr(User<RepositoryType>(insertData));
			}
		}
	};
};