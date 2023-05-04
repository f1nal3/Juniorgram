#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <string>
#include <Models/UnifyedModel.hpp>

namespace DataAccess
{
template <class TEnum, class TResult = pqxx::const_result_iterator>
class PostgreModel : public Models::UnifiedModel<TEnum, TResult>
{
public:
    PostgreModel(const std::string& modelName, size_t amountFields)
        : Models::UnifiedModel<TEnum, TResult>(modelName, amountFields)
    {
    }

public:
    void fill(const TResult& responce) const final
    {
        for (auto respIter = responce.begin(); respIter != responce.end(); ++respIter)
        {
            this->_data[this->toEnum(respIter.name())] = respIter.template as<std::string>();
        }
    }
    using type = TResult;
protected:
    virtual TEnum getNumEnum(size_t) const = 0;
};
}  // namespace DataAccess
