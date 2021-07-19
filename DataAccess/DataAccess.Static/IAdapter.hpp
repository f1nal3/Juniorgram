#pragma once
#include <string_view>
#include <optional>
#include <any>

#include <pqxx/pqxx>

namespace DataAccess
{
	class IAdapter
	{
	public:

		/** @brief Method for getting Adapter instances. \
		*	@details This method return the instance with your options. \
		*	 Like: dbname=... user=... hostaddr=... port=... ... . \
		*	 There are another options like: ssh, password, etc. \
		*	 This options must be passed as a single string. \
		*	 By default: \
		     (dbname=juniorgram user=postgres hostaddr=127.0.0.1 port=5432).
		*	@param Params for connecting to the database.
		*	@return Adapter instance: std::shared_ptr<...>
		*/
		template<typename T>
  static std::shared_ptr<T> getInstance(const std::string_view& options = {})
  {
			return T::Instance(options);
		}

		virtual std::optional<std::any> query(const std::string_view& query) = 0;

		virtual bool isConnected(void) const = 0;

		virtual void closeConnection(void) = 0;

		virtual ~IAdapter(){};
	};
}
