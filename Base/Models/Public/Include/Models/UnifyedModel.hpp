#pragma once

#include <map>
#include <vector>
#include <string>

namespace Models
{
using FieldNames = std::initializer_list<std::string>;

template <typename Enum>
using FieldData = std::vector<std::tuple<Enum, size_t, std::string>>;

template <typename TKey>
class Comparator
{
public:
    bool operator()(const TKey& first, const TKey& second) const noexcept
    {
        return static_cast<size_t>(first) < static_cast<size_t>(second);
    }
};

template <typename TKey, typename TData = std::string, typename Comp = Comparator<TKey>>
class Map : public std::map<TKey, TData, Comp>
{
};

template <class TEnum, class TResult>
class UnifyiedModel
{
private:
    std::string              _modelName;
    size_t                   _amountOfFields;
    std::vector<std::string> _fieldNames;

protected:
    Map<TEnum>       _data;
    FieldData<TEnum> _fieldData;  /// maybe vector<tuple<T,size_t,string>> -> map<T,std::pair<size_t,string>> faster access

public:
    UnifyiedModel(const std::string& modelName, FieldNames names, size_t amountFields)
        : _modelName(modelName), _amountOfFields(amountFields), _fieldNames(names)
    {
    }

    const std::string getModelName() const noexcept { return _modelName; }

    const size_t enumToNum(TEnum anyEnum) const noexcept
    {
        return std::get<1>(*std::find_if(_fieldData.begin(), _fieldData.end(), [anyEnum](std::tuple<TEnum, size_t, std::string> tpl) {
            if (std::get<0>(tpl) == anyEnum)
                return true;
            else
                return false;
        }));
    }

    const std::string& fieldName(TEnum anyEnum) const noexcept
    {
        return std::get<2>(*std::find_if(_fieldData.begin(), _fieldData.end(), [anyEnum](std::tuple<TEnum, size_t, std::string> tpl) {
            if (std::get<0>(tpl) == anyEnum)
                return true;
            else
                return false;
        }));
    }

    const std::vector<TEnum>& getAllFields()
    {
        std::vector<TEnum> temp;
        for (size_t i{0}; i < _fieldData.size(); ++i) temp.emplace_back(std::get<0>(_fieldData[i]));

        return temp;
    }

    virtual void fillMap(const TResult& response) = 0;  

    std::string& operator[](TEnum anyEnum) { return _data[anyEnum]; }

    virtual ~UnifyiedModel() = default;

protected:
    void init()
    {
        size_t counter{0};
        while (counter < _amountOfFields)
        {
            _fieldData.emplace_back(std::make_tuple<TEnum, size_t, std::string>(this->getNumEnum(counter), std::move(counter),
                                                                                (_fieldNames.begin() + counter)->c_str()));
            _data.insert(std::pair<TEnum, std::string>(this->getNumEnum(counter), std::string{}));
            ++counter;
        }
        _fieldNames.clear();
    }

private:
    /*
     * @method getNumEnum
     * @param size_t num -> used to find all objects from specific enum class
     * @returns Object of specific enum class
     */
    virtual TEnum getNumEnum(size_t num) const = 0;
};
}  // namespace Models