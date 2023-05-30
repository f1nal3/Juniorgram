#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <string>
#include <Models/UnifyedModel.hpp>


namespace DataAccess
{

class PGModelFiller
{
public:
    PGModelFiller() = default;
    
public:
    template <typename TEnum, class TResult = pqxx::const_result_iterator>
    void fill(const TResult& response, Models::UnifiedModel<TEnum>* model) const
    {
        for (auto respIter = response.begin(); respIter != response.end(); ++respIter)
        {
            (*model)[model->toEnum(respIter.name())] = respIter.template as<std::string>();
        }
    }

public:
    ~PGModelFiller() = default;
};
}  // namespace DataAccess
