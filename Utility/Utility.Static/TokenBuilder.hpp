    #pragma once 

#include <tuple>
#include <string>
#include <iostream>
#include <variant>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <nlohmann/json.hpp>
#include <cassert>
#include <algorithm>

#include "Utility/WarningSuppression.hpp"

#include "Cryptography.hpp"

namespace Utility
{
using json = nlohmann::json;

constexpr short tokenExpiredTime = 3600; 

suppressWarning(4100, Init)
struct GetHeader
{
    template<typename T>
    GetHeader(const std::initializer_list<std::pair<std::string_view, T>>& init_list);

    json mJsonObj;
};

struct GetPayload
{
    template <typename T>
    GetPayload(const std::initializer_list<std::pair<std::string_view, T>>& init_list);

    json mJsonObj;
};

struct GetSignature 
{
};
restoreWarning


template <typename S, typename... Handlers>
struct TokenBuilder;

template <typename S>
struct TransitionTo
{
    using TargetState = S;
};

template <typename... States, typename... Handlers>
struct TokenBuilder<std::tuple<States...>, Handlers...> : Handlers...
{
    using Handlers::operator()...;

    template <typename... H>
    TokenBuilder(H&... h);

    template <typename E>
    void onEvent(E&& e);

    std::tuple<States...> states;
    std::variant<States*...> currentState = &std::get<0>(states);
};

suppressWarning(4239, Init)
template <typename S, typename... Handlers>
auto makeTokenBuilder(Handlers&&... h);
restoreWarning

struct BuildHeader
{
};
struct BuildPayload
{
};
struct BuildSignature
{
};
 
std::string buildToken(const std::shared_ptr<Network::Connection>& client);

}  // namespace Utility
