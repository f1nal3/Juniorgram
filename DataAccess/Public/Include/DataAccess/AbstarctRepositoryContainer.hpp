#pragma once
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <stdexcept>

#include "IMasterRepository.hpp"

namespace DataAccess
{
	/* @class AbstarctRepositoryContainer
	*  @brief An abstarct class for server and client global repositories.
	*/
	class AbstarctRepositoryContainer
	{
        public:

			using RealType = std::string;
			using IType    = std::shared_ptr<IMasterRepository>;

        protected:

			std::shared_ptr<IAdapter> _adapter;
			std::map<RealType, IType> _container;

		public:

			AbstarctRepositoryContainer(std::shared_ptr<IAdapter> adapter) : _adapter(adapter), _container() {}

			/* @brief Method for register new repositories.
			*  (You should pass, firstly, an interface and
			*  after an implementation of this repository)
			*/
			template <typename TInterface, typename TRepository>
			void registerRepository(void)
			{
				static_assert(std::is_base_of_v<TInterface, TRepository>, "Repository does not implement current Interface!");

				const char* typeInfo = typeid(TInterface).name();

				if (_container.find(typeInfo) != _container.end())
				{
					throw std::runtime_error(std::string("This interface has already registered! - ") + typeInfo);
				}

				_container.emplace(typeInfo, std::make_shared<TRepository>(_adapter));
			}

			/* @brief Method for get a repositories.
            *  (You should pass an interface)
            */
			template <typename TInterface>
            std::shared_ptr<TInterface> getRepository(void)
            {
				const char* typeInfo = typeid(TInterface).name();

				if (_container.find(typeInfo) == _container.end())
				{
					throw std::runtime_error(std::string("This repository was not registered! - ") + typeInfo);
				}

				return std::dynamic_pointer_cast<TInterface>(_container[typeInfo]);
            }
	};
}
