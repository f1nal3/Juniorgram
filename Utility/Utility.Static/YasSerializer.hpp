#pragma once
// clang-format off
suppressWarning(4996, -Wdeprecated-declarations)
suppressWarning(4458, -Wshadow)
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/buffers.hpp>
#include <yas/mem_streams.hpp>
#include <yas/object.hpp>
#include <yas/serialize.hpp>
#include <yas/std_types.hpp>
#include <yas/types/std/string.hpp>
#include <yas/types/std/vector.hpp>
restoreWarning
restoreWarning
// clang-format on

#include "Network/Primitives.hpp"

namespace Network {
    /** @class YasSerializer
     *  @brief binary serialization class using YAS library.
     */
	class YasSerializer
	{
		YasSerializer()                     = delete;
		YasSerializer(const YasSerializer&) = delete;
		YasSerializer(YasSerializer&&)      = delete;
		YasSerializer& operator=(const YasSerializer&) = delete;
		YasSerializer& operator=(YasSerializer&&) = delete;

	private:
		constexpr static std::size_t flags =
			yas::mem | yas::binary | yas::no_header | yas::compacted;
	
	public:
        /**
        * @brief Method for binary serialization of messages.
        * @param msg - buffer that will contain serialized message data.
        * @param data - variable that contains data that should be serialized.
        */
		template <typename T>
		static void serialize(yas::shared_buffer& msg, const T& data)
		{
			// clang-format off
			suppressWarning(4127, -Wtype-limits)
				msg = yas::save<flags>(data);
			restoreWarning
		}
		// clang-format on

        /**
        * @brief Method for binary deserialization of messages.
        * @param source - variable that contains data that should be deserialized.
        * @param data - variable that will contain deserialized message data.
        */
		template <typename T>
		static void deserialize(const yas::shared_buffer source, T& data)
		{
			// clang-format off
			suppressWarning(4127, -Wtype-limits)
				yas::load<flags>(source, data);
			restoreWarning
		}
		// clang-format on
	};
}
