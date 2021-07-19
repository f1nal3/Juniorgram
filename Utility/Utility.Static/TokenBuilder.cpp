#include <Network.Static/Connection.hpp>
#include "TokenBuilder.hpp"

namespace Utility
{
    template <typename T>
    GetHeader::GetHeader(const std::initializer_list<std::pair<std::string_view, T>>& init_list)
    {
        std::for_each(
            init_list.begin(), init_list.end(), [this](std::pair<std::string_view, T> currPair) {
                mJsonObj[std::string(currPair.first.data(), currPair.first.size())] = currPair.second;
            });
    }

    template <typename T>
    GetPayload::GetPayload(const std::initializer_list<std::pair<std::string_view, T>>& init_list)
    {
        std::for_each(init_list.begin(), init_list.end(),
                      [this](std::pair<std::string_view, T> currPair) {
                          mJsonObj[std::string(currPair.first.data(), currPair.first.size())] =
                              currPair.second;
                      });
    }
    
    template <class... States, class... Handlers>
    template <class... H>
    TokenBuilder<std::tuple<States...>, Handlers...>::TokenBuilder(H&... h) : Handlers(h)...
    {
    }

    template <class... States, class... Handlers>
    template <class E>
    void TokenBuilder<std::tuple<States...>, Handlers...>::onEvent(E&& e)
    {
        std::visit(
            [this, &e](auto statePtr) {
                if constexpr (std::is_invocable_v<TokenBuilder, decltype(*statePtr), E&&>)
                {
                    using ResultType = std::invoke_result_t<TokenBuilder, decltype(*statePtr), E&&>;
                    if constexpr (std::is_same_v<ResultType, void>)
                    {
                        (*this)(*statePtr, std::forward<E>(e));
                    }
                    else
                    {
                        /*useless transitionTo. Maybe i'll find useful palce for this.*/
                        auto transitionTo = (*this)(*statePtr, std::forward<E>(e));
                        currentState      = &std::get<typename ResultType::TargetState>(states);
                    }
                }
                else
                {
                    assert(false && "Òo rules invoked!\n");
                }
            },
            currentState);
    }
   
    template <typename S, typename... Handlers>
    auto makeTokenBuilder(Handlers&&... h)
    {
        return TokenBuilder<S, std::decay_t<Handlers>...>{std::forward<Handlers>(h)...};
    }

    std::string buildToken(const std::shared_ptr<Network::Connection>& client)
    {
        using namespace std::string_view_literals;

        std::string finaleJSONToken;

        suppressWarning(4100, Init) suppressWarning(4239, Init) auto tokenBuilder =
            makeTokenBuilder<std::tuple<BuildHeader, BuildPayload, BuildSignature>>(
                [&finaleJSONToken](BuildHeader& s, GetHeader event) -> TransitionTo<BuildPayload> {
                    finaleJSONToken = Coding::getBASE64CodedValue(event.mJsonObj.dump()) + '.';
                    return {};
                },

                [&finaleJSONToken](BuildPayload& s,
                                   GetPayload event) -> TransitionTo<BuildSignature> {
                    finaleJSONToken += Coding::getBASE64CodedValue(event.mJsonObj.dump()) + '.';
                    return {};
                },

                [&finaleJSONToken, &client](BuildSignature& s,
                                            GetSignature event) -> TransitionTo<BuildHeader> {
                    std::string signedTokenPart = Signing::signData(client, finaleJSONToken);

                    finaleJSONToken += signedTokenPart;
                    return {};
                });

        tokenBuilder.onEvent(Utility::GetHeader{
            {std::pair{"alg"sv,
                       CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::StaticAlgorithmName()},
             std::pair{"typ"sv, std::string("JWT")}}});

        tokenBuilder.onEvent(Utility::GetPayload{
            {std::pair{"exp"sv, std::to_string((std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) + tokenExpiredTime))},
             std::pair{"id"sv, std::to_string(client->getID())}, std::pair{"ip"sv, client->getIP()},
             std::pair{"iat"sv, std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))},
             std::pair{"os"sv, []() -> std::string {
                        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                                                   return "Windows";
                        #elif __APPLE__ 
                                                            return "MacOS";
                        #elif __linux__ 
                                                            return "Linux" ;
                        #endif
                       }()}}});

        tokenBuilder.onEvent(Utility::GetSignature{});

        restoreWarning 
        restoreWarning 
            
        return finaleJSONToken;
    }

}
