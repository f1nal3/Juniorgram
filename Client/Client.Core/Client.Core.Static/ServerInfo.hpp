#include <string_view>

namespace ServerInfo
{
/// Server address
constexpr std::string_view address = "127.0.0.1";
/// Server port
/// 65001 can be used for test server
constexpr std::uint16_t port = 8080;
}  // namespace ServerInfo
