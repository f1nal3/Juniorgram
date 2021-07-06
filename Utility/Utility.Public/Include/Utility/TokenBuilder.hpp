#pragma once 

#include "Utility/WarningSuppression.hpp"

#include <tuple>
#include <string>
#include <iostream>
#include <variant>
#include <initializer_list>

namespace Utility
{
suppressWarning(4100, Init)
struct GetHeader
{
    GetHeader(const std::initializer_list<std::string_view>& init_list) 
    {

    }
};

struct GetPayload
{
    GetPayload(const std::initializer_list<std::string_view>& init_list)
    {
    
    }
};

struct GetSignature
{
    GetSignature(const std::initializer_list<std::string_view>& init_list)
    {
    
    }
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
    TokenBuilder(H&... h) : Handlers(h)...
    {
    }

    template <typename E>
    void onEvent(E&& e)
    {
        std::visit(
            [this, &e](auto statePtr) {
                if constexpr (std::is_invocable_v<TokenBuilder, decltype(*statePtr), E&&>)
                {
                    using ResultType = std::invoke_result_t<TokenBuilder, decltype(*statePtr), E&&>;
                    if constexpr (std::is_same_v<ResultType, void>)
                    {
                        (*this)(*statePtr, std::forward<E>(e));
                        std::cout << "(no transition)\n";
                    }
                    else
                    {
                        auto transitionTo = (*this)(*statePtr, std::forward<E>(e));
                        currentState      = &std::get<typename ResultType::TargetState>(states);
                        std::cout << "(transitioned to " << currentState.index() << ")\n";
                    }
                }
                else
                {
                    std::cout << "(no rules invoked)\n";
                }
            },
            currentState);
    }

    std::tuple<States...> states;
    std::variant<States*...> currentState = &std::get<0>(states);
};

suppressWarning(4239, Init)
template <typename S, typename... Handlers>
auto makeTokenBuilder(Handlers&&... h)
{
    return TokenBuilder<S, std::decay_t<Handlers>...>{std::forward<Handlers>(h)...};
}
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

auto getTokenBuilder()
{
    suppressWarning(4100, Init)
    auto sm = makeTokenBuilder<std::tuple<BuildHeader, BuildPayload, BuildSignature>>(
        [](BuildHeader& s, GetHeader event) -> TransitionTo<BuildPayload> { return {}; },
        [](BuildPayload& s, GetPayload event) -> TransitionTo<BuildSignature> { return {}; },
        [](BuildSignature& s, GetSignature event) {});
    return sm;
    restoreWarning
}

}  // namespace Utility
