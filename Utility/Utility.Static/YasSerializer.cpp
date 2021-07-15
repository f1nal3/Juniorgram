#include "YasSerializer.hpp"

namespace Utility
{
    template <typename T>
    static SerializedState YasSerializer::serialize(yas::shared_buffer& msg, const T& data)
    {
        try
        {
            // clang-format off
				    suppressWarning(4127, -Wtype-limits)
					    msg = yas::save<flags>(data);
				    restoreWarning
                }
        // clang-format on
        catch (const std::exception& e)
        {
            std::cout << "Serialization error\n";
            std::cout << e.what() << '\n';

            return SerializedState::FAILURE;
        }

        return SerializedState::SUCCESS;
    }

    template <typename T>
    static SerializedState deserialize(const yas::shared_buffer source, T& data)
    {
        try
        {
            suppressWarning(4127, -Wtype - limits) yas::load<flags>(source, data);
            restoreWarning
            // clang-format off
			         }
        // clang-format on
        catch (const std::exception& e)
        {
            std::cout << "Deserialization error\n";
            std::cout << e.what() << '\n';

            return SerializedState::FAILURE;
        }

        return SerializedState::SUCCESS;
    }
}