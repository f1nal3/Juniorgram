#pragma once
#include <pqxx/pqxx>
#include <algorithm>
#include <map>
#include <Models/UnifyedModel.hpp>

namespace DataAccess
{
template<class TEnum, class TResult = pqxx::result>
class PostgreModel: public Models::UnifyiedModel<TEnum,TResult>
{
public:
    virtual TEnum getNumEnum(size_t num) const = 0;

    PostgreModel(const std::string& modelName, Models::FieldNames names, size_t amountFields):
        Models::UnifyiedModel<TEnum,TResult>(modelName,names,amountFields) 
    {}

public:
    void fillMap(const TResult& responce) override 
    {
        auto iter      = responce.begin();
        auto fieldIter = _fieldData.begin();

        for (auto innerIter = iter.begin(); innerIter != iter.end(); ++innerIter, ++fieldIter)
        {
            _data[std::get<0>(*fieldIter)] = innerIter.as<std::string>();
        }
    }
};
}
