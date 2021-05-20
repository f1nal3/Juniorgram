#pragma once
#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>
#include <yas/buffers.hpp>
#include <yas/mem_streams.hpp>
#include <yas/object.hpp>
#include <yas/serialize.hpp>
#include <yas/std_types.hpp>
#include <yas/types/std/string.hpp>
#include <yas/types/std/vector.hpp>

#include "Network/Primitives.hpp"

namespace Network {
	class YasSerializer
	{
	private:
		constexpr static std::size_t flags =
			yas::mem | yas::binary | yas::no_header | yas::compacted;
	
	public:
		template <typename T>
		static void serialize(yas::shared_buffer& msg, const T& data)
		{
			// clang-format off
			suppressWarning(4127, -Wtype-limits)
				msg = yas::save<flags>(data);
			restoreWarning
		}
		// clang-format on
		
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
