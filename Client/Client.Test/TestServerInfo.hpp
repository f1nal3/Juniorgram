#include <string_view>

namespace TestServerInfo
{
namespace Address
{
    constexpr std::string_view remote = "104.40.239.183";
    constexpr std::string_view local  = "127.0.0.1";
}
namespace Port
{
    constexpr std::uint16_t test       = 65001;
    constexpr std::uint16_t production = 65003;
}
}  /// namespace TestServerInfo
