#include <string_view>

constexpr std::uint16_t test_server = 65001;
constexpr std::uint16_t production_server = 65003;

namespace ServerInfo
{
/// Server address
constexpr std::string_view address = "104.40.239.183";
/// Server port
/// you can change the needed server using two variables above
/// also, test means "fresh" e.g. you are on master now
/// production means based on release commit that is tagged.
constexpr std::uint16_t port = production_server;
}  // namespace ServerInfo
