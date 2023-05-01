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
using Map = std::map<TKey, TData, Comp>;

template <class TEnum, class TResult>
class UnifyedModel
{
public:
    UnifyedModel(const std::string& modelName, size_t amountFields)
        : _modelName(modelName), _amountOfFields(amountFields)
    {
    }

    const std::string& getModelName() const noexcept { return _modelName; }

    const std::string& fieldName(TEnum anyEnum) noexcept
    {
        return _fieldData[anyEnum];
    }

    virtual void fillMap(const TResult& response) = 0;  

    std::string& operator[](TEnum anyEnum) { return _data[anyEnum]; }

    TEnum toEnum(const std::string& fieldName)
    {
        return std::find_if(_fieldData.begin(), _fieldData.end(), [&fieldName](auto pair)
                                      {
                                          if (pair.second == fieldName)
                                              return true;
                                          else
                                              return false;
                                      })->first;
    }

    virtual ~UnifyedModel() = default;

protected:
    void init(const FieldNames& fieldNames)
    {
        size_t counter{0};
        while (counter < _amountOfFields)
        {
            _fieldData.insert(std::pair<TEnum, std::string>(this->getNumEnum(counter),
                                                                  *(fieldNames.begin() + counter)));

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
    std::string              _modelName;
    size_t                   _amountOfFields;

protected:
    mutable Map<TEnum>       _data;
    Map<TEnum>               _fieldData;

};
}  // namespace Models
