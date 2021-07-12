#pragma once 

#include <tuple>
#include <string>
#include <iostream>
#include <variant>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <Utility.Static/Cryptography.hpp>

namespace Utility
{

using json = nlohmann::json;

constexpr short tokenExpiredTime = 3600; 

suppressWarning(4100, Init)
struct GetHeader
{
    template<typename T>
    GetHeader(const std::initializer_list<std::pair<std::string_view, T>>& init_list) 
    {
        std::for_each(init_list.begin(), init_list.end(), [this](std::pair<std::string_view, T> currPair)
        { 
            j[std::string(currPair.first.data(), currPair.first.size())] = currPair.second; 
        });
    }

    json j;
};

struct GetPayload
{
    template <typename T>
    GetPayload(const std::initializer_list<std::pair<std::string_view, T>>& init_list)
    {
        std::for_each(
            init_list.begin(), init_list.end(), [this](std::pair<std::string_view, T> currPair)
        { 
              j[std::string(currPair.first.data(), currPair.first.size())] = currPair.second; 
        });
    }

    json j;
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
    TokenBuilder(H&... h) : Handlers(h)...
    {
    }

    template <typename E>
    void onEvent(E&& e)
    {
        std::visit([this, &e](auto statePtr) 
        {
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
                        /*useless transitionTo. Maybe i'll find useful palce for this.*/
                        auto transitionTo = (*this)(*statePtr, std::forward<E>(e));
                        currentState      = &std::get<typename ResultType::TargetState>(states);
                        std::cout << "(transitioned to " << currentState.index() << ")\n";
                    }
                }
                else
                {
                    std::cout << "(no rules invoked)\n";
                }
        },currentState);
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
 
auto buildToken(const std::shared_ptr<Network::Connection>& client)
{
    using namespace std::string_view_literals;

    std::string finaleJSONToken;
  
    suppressWarning(4100, Init) 
    suppressWarning(4239, Init)
    auto tokenBuilder = makeTokenBuilder<std::tuple<BuildHeader, BuildPayload, BuildSignature>>
    (
        [&finaleJSONToken](BuildHeader& s, GetHeader event) -> TransitionTo<BuildPayload>
        { 
            finaleJSONToken = Coding::getBASE64CodedValue(event.j.dump()) + '.';
            return {}; 
        },

        [&finaleJSONToken](BuildPayload& s, GetPayload event) -> TransitionTo<BuildSignature>
        { 
            finaleJSONToken += Coding::getBASE64CodedValue(event.j.dump()) + '.';
            return {};
        },
        
        [&finaleJSONToken, &client](BuildSignature& s, GetSignature event) -> TransitionTo<BuildHeader>
        { 
            std::string signedTokenPart = Signing::signData(client, finaleJSONToken);
                
            finaleJSONToken += signedTokenPart;
            return {};
        });

    tokenBuilder.onEvent(Utility::GetHeader
        {{
            std::pair{"alg"sv, CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::StaticAlgorithmName()}, 
            std::pair{"typ"sv, std::string("JWT")}
        }});

    tokenBuilder.onEvent(Utility::GetPayload
        {{
        std::pair{"exp"sv, std::to_string((std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) + tokenExpiredTime))},
        std::pair{"id"sv, std::to_string(client->getID())},
        std::pair{"ip"sv, client->getIP()},
        std::pair{"iat"sv, std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))},
        std::pair{"os"sv, []() -> std::string 
                                {
                                #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) 
                                    return "Windows";
                                #elif __APPLE__ 
                                    return "MacOS";
                                #elif __linux__ 
                                    return "Linux" ; 
                                #endif
                                }()
                  }
        }});

    tokenBuilder.onEvent(Utility::GetSignature{});
    
    restoreWarning
    restoreWarning 
    return finaleJSONToken;
}

}  // namespace Utility
