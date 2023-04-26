#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <Models/UnifyedModel.hpp>

namespace DataAccess
{
template <class TEnum, class TResult = pqxx::const_result_iterator>
class PostgreModel : public Models::UnifyedModel<TEnum, TResult>
{
public:
    PostgreModel(const std::string& modelName, size_t amountFields)
        : Models::UnifyedModel<TEnum, TResult>(modelName, amountFields)
    {
    }

public:
    virtual void fillMap(const TResult& responce) override
    {
        auto fieldIter = this->_fieldData.begin();

        for (auto innerIter = responce.begin(); innerIter != responce.end(); ++innerIter, ++fieldIter)
        {
            this->_data[fieldIter->first] = innerIter.template as<std::string>();
        }
    }

protected:
    virtual TEnum getNumEnum(size_t) const = 0;
};
}  // namespace DataAccess
