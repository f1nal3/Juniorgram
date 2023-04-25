#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <Models/UnifyedModel.hpp>

namespace DataAccess
{
template <class TEnum, class TResult = pqxx::result>
class PostgreModel : public Models::UnifyedModel<TEnum, TResult>
{
public:
    PostgreModel(const std::string& modelName, Models::FieldNames names, size_t amountFields)
        : Models::UnifyedModel<TEnum, TResult>(modelName, names, amountFields)
    {
    }

public:
    virtual void fillMap(const TResult& responce) override
    {
        auto iter      = responce.begin();
        auto fieldIter = this->_fieldData.begin();

        for (auto innerIter = iter.begin(); innerIter != iter.end(); ++innerIter, ++fieldIter)
        {
            this->_data[std::get<0>(*fieldIter)] = innerIter.template as<std::string>();
        }
    }

protected:
    virtual TEnum getNumEnum(size_t) const = 0;
};
}  // namespace DataAccess
