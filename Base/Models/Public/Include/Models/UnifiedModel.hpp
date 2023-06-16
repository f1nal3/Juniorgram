#pragma once

#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <type_traits>

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
    
    UnifiedModel(const UnifiedModel& other) 
    {
        _data = other._data;
        _amountOfFields = other._amountOfFields;
        _modelName = other._modelName;
    }

    UnifiedModel(UnifiedModel<TEnum>&& other) noexcept
    {
        _data = std::move(other._data);
        other._data.clear();

        _modelName = std::move(other._modelName);
        
        _amountOfFields = std::move(other._amountOfFields);
    }

    /*
    * @brief Method getModelName
    * @param None
    * @return const char* -> name of the model (equal to name of the table)
    */
    const char* getModelName() const noexcept { return _modelName.data(); }

    /*
    * @brief Method resolveName
    * @param TEnum anyEnum -> Specific enum entity for the class
    * @return std::string -> field name
    */
    std::string resolveName(TEnum anyEnum)const noexcept
    {
        return { _data[anyEnum].first.begin(), _data[anyEnum].first.end() };
    }

    std::string& operator[](TEnum anyEnum) const { return _data[anyEnum].second; }
    
    /*
    * @brief Method toEnum
    * @param string_view fieldName
    * @return TEnum -> Field of specific enum, in other words returns first object from relation pair <EnumField, fieldName>
    * @details Used in PGModelFiller for automated access in pqxx::row
    */
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

    /*
    * @brief Method makeColumnDataPair
    * @param None
    * @returns Vector of pairs <field_name, field_value>
    * @details Used in QueryBuilder to make relations between models and SQLInsert more object-related
    */
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
    /*
    * @brief Method fillStartFields
    * @param vector of pair<TEnum, std::string> where string - value of field, enum - what field is it
    * @details Used to fill class variable which holds data with incoming one
    */
    void fillStartFields(const std::vector<std::pair<TEnum, std::string>>& insertData)
    {
        std::for_each(insertData.begin(), insertData.end(), [this](const auto& pair)
                      {
                          this->_data[pair.first].second = pair.second;
                      });
    }
    
    /*
    * @brief Method init
    * @param const FieldNames& fieldNames -> vector of pairs of enum<->field name
    * @details Used to initialize class field(map) with specific enum of model and model field names
    */
    void init(const FieldNames& fieldNames)
    {       
        for (size_t counter{ 0 }; counter < _amountOfFields; ++counter)
            _data.insert({ this->getNumEnum(counter), {fieldNames[counter], std::string{}} });
    }

    template <template<typename> class Derived>
    void moveOperation(Derived<TEnum>&& other) noexcept
    {
        if (std::is_base_of_v<UnifiedModel<TEnum>, Derived<TEnum>>)
        {
            _data = std::move(other._data);
        }
    }

private:
    /*
     * @brief Method getNumEnum
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
