#pragma once

#include <map>
#include <vector>
#include <string>
#include <string_view>

namespace Models
{
using FieldNames = std::vector<std::string_view>;

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
using Map = std::map<TKey, TData, Comp>;

template <typename TEnum>
class UnifiedModel
{
public:
    UnifiedModel(std::string_view modelName, size_t amountFields): _modelName(modelName), _amountOfFields(amountFields)
    {
    }

    std::string_view getModelName() const noexcept { return _modelName; }

    std::string fieldName(TEnum anyEnum)const noexcept
    {
        return { _fieldData[anyEnum].begin(),_fieldData[anyEnum].end()};
    }

    std::string& operator[](TEnum anyEnum) const { return _data[anyEnum]; }
   
    virtual ~UnifiedModel() = default;

    TEnum toEnum(std::string_view fieldName) const
    {
        return std::find_if(_fieldData.begin(), _fieldData.end(), [&fieldName](auto pair)
                            {
                                if (pair.second == fieldName)
                                    return true;
                                else
                                    return false;
                            })->first;
    }

protected:
    void init(const FieldNames& fieldNames)
    {
        size_t counter{0};
        while (counter < _amountOfFields)
        {
            _fieldData.insert(std::pair<TEnum, std::string_view>(this->getNumEnum(counter),
                                                                   fieldNames[counter]));

            _data.insert(std::pair<TEnum, std::string>(this->getNumEnum(counter), std::string{}));
            ++counter;
        }
    }

private:
    /*
     * @method getNumEnum
     * @param size_t num -> used to find all objects from specific enum class
     * @returns Object of specific enum class
     */
    virtual TEnum getNumEnum(size_t num) const = 0;
  
private:
    std::string_view         _modelName;
    size_t                   _amountOfFields;

protected:
    mutable Map<TEnum>                        _data;
    mutable Map<TEnum,std::string_view>       _fieldData;

};
}  // namespace Models
