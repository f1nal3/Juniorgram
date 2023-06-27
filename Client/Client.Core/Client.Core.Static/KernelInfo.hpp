#include <string_view>

namespace KernelInfo
{
/// Kernel address
namespace Address
{
    constexpr std::string_view remote = "104.40.239.183";
    constexpr std::string_view local  = "127.0.0.1";
}
/// Kernel port
/// you can change the needed kernel using one of the variables below
/// also, test means "fresh" e.g. you are on master now 
/// production means based on release commit that is tagged.
namespace Port
{
    constexpr std::uint16_t test       = 65001;
    constexpr std::uint16_t production = 65003;
}
}  // namespace KernelInfo
