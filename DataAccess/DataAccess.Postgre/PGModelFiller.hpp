#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <string>
#include <Models/UnifiedModel.hpp>


namespace DataAccess
{

class PGModelFiller
{
public:
    PGModelFiller() = default;
    
    template <typename TEnum, class TResult = pqxx::const_result_iterator>
    void fill(const TResult& response, const Models::V2::UnifiedModel<TEnum>* model) const
    {
        if(!std::empty(response))
            for (auto respIter = std::cbegin(response); respIter != std::cend(response); ++respIter)
            {
                (*model)[model->toEnum(respIter.name())] = respIter.template as<std::string>();
            }
    }

    ~PGModelFiller() = default;
};
}  // namespace DataAccess
