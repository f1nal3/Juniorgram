#pragma once

#include <map>
#include <vector>
#include <string>
#include <string_view>

#include <DataAccess/SQLStatements.hpp>

namespace Models
{
using FieldNames = std::vector<std::string_view>;
using InsertData = std::vector<std::pair<std::string_view, std::string_view>>;

template <typename TKey>
class Comparator
{
public:
    bool operator()(const TKey& first, const TKey& second) const noexcept
    {
        return static_cast<size_t>(first) < static_cast<size_t>(second);
    }
};

template <typename TKey, typename Comp = Comparator<TKey>>
using Map = std::map<TKey, std::pair<std::string_view, std::string>, Comp>;

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
        return { _data[anyEnum].first.begin(), _data[anyEnum].first.end() };
    }

    std::string& operator[](TEnum anyEnum) const { return _data[anyEnum].second; }
    
    TEnum toEnum(std::string_view fieldName) const
    {
        return std::find_if(_data.begin(), _data.end(), [&fieldName](const auto& pair)
                            {
                                if (pair.second.first == fieldName)
                                    return true;
                                else
                                    return false;
                            })->first;
    }

    InsertData makeColumnDataPair() const
    {
        InsertData pairs;
        pairs.reserve(_amountOfFields);

        std::for_each(_data.begin(), _data.end(), [&pairs](const auto& pair)
                      {
                          if (!std::empty(pair.second.second))
                              pairs.emplace_back(pair.second);
                      }
        );
        return pairs;
    }

    virtual ~UnifiedModel() = default;

protected:
    void fillStartFields(const std::vector<std::pair<TEnum, std::string>>& insertData)
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          this->_data[pair.first].second = pair.second;
                      });
    }
      
    void init(const FieldNames& fieldNames)
    {       
        for (size_t counter{ 0 }; counter < _amountOfFields; ++counter)
            _data.insert({ this->getNumEnum(counter), {fieldNames[counter], std::string{}} });
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

    mutable Map <TEnum>      _data;
};
}  // namespace Models
