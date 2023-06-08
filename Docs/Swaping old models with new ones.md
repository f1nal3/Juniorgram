# How to change old model dependency on the new model structure

## 1. You should find out exists or not the class which will change the old model

> You need to check it in 'Base/Models/Models.hpp'

### 1.1 If this class does not exist.

> You ***must*** know the ***name*** of the model(table), ***how many fields*** it has and their names

> While you are creating the model, everything you write should be located in file mentioned above

- First of all you should create enum class in which all fields wiil be represented as enum

Example:
```
enum class BookData
{
	AUTHOR,
	NAME,
	PAGES
};
```

- Create a template class with talking name, inheritance the basic template class **UnifiedModel**
- Make a template argument as a default one with enum class which you've created
- Create two constructors - default and __filling__
- Follow the example

> Template is the one that is in charge almost of everything at start level

```
template <TEnum = BookData>
class Book: public UnifiedModel<TEnum>
{
public:
	Book(std::string_view modelName = "books", const Models::FieldNames& names = { "author", "name", "pages" })
        : UnifiedModel<TEnum>(modelName, names.size())
    {     
        this->init(names);
    }

    explicit Book(const std::vector<std::pair<TEnum, std::string>>& insertData) : Book()
    {
        this->fillStartFields(insertData);
    }
};
```

- After that, inside **protected** scope, override method **getNumEnum**
- Syntax -> TEnum getNumEnum(size_t) const final

> You ***must*** make the ***order of names*** that you put inside ***default constructor and switch*** below totally ***equal***, the amount likewise

```
protected:
BookData getNumEnum(size_t num)const final
{
    switch(num)
    {
        case 0: return BookData::AUTHOR;
        case 1: return BookData::NAME;
        case 2: return BookData::PAGES;
        default: 
        Base::Logger::FileLogger::getInstance().
            error("Overrided method does not deal with this amount of fields given in constructor");
    }
}
```

## 2. When class exists, let's do the rest

- In DataAccess/Public/IServerRepository.hpp you should find every mention of old model and change it on the new one
- That was a start point, right now you have to change it everywhere in the interfaces

> Right now is
> - DataAccess.Postgre/PostgreRepositories.hpp/cpp
> - DataAccess.Mock/MockRepositories.hpp/cpp

Example:

Before:
```
    virtual Utility::ChannelCreateCodes createChannel(const Models::ChannelInfo& channel) = 0;
```

After:
```
    virtual Utility::ChannelCreateCodes createChannel(const Models::New::Channel<>& channel) = 0;
```

> Also you must change logic inside Mock, but that's simple one, rules will be described below


## 3. Changing the repository method. Main improvements that you can do


### - You don't need to write the hard-coded name of the table:

Before:
```
    _pTable->changeTable("channels");
```

After:
```
    _pTable->changeTable(channel.getModelName());
```


### - No more raw strings inside QueryBuilder methods:

Before:
```
    auto findChannel = _pTable->Select()
        ->columns({"channel_name"})
        ->Where("channel_name = '" + channel._channelName + "'")
        ->execute();
```

After:
```
    auto findChannel = _pTable->Select()
        ->columns({ channel.resolveName(ChannelData::CHANNEL_NAME)})
        ->Where(channel.resolveName(ChannelData::CHANNEL_NAME)+ "= '" + channel[ChannelData::CHANNEL_NAME] + "'")
        ->execute();
```


### - No more any type of casts inside method logic

> This task has moved to the separate class - ModelFiller (right now only PGFiller)
> This class knows how to cast and what to cast, and fills model with it

Before('findChannel' - is a returning object from _pTable->...->execute()):
```
    auto creatorlID  = findChannel.value()[0][0].as<uint64_t>();
    auto channelID   = findChannel.value()[0][1].as<uint64_t>();
```

After:
```
    _filler->fill(findChannel->begin(), &channel);
```

> We use __returningObjectName__->begin() only when we need to fill only one model or returning value consists of one iterator
> Otherwise create a loop to fill a vector of models


### - No more tuples inside method logic

> Insert method can use the object itself to build the query

Before:
```
    std::tuple channelData{std::pair{"channel_name", channel._channelName},
                           std::pair{"creator_id", channel._creatorID},
                           std::pair{"user_limit", 1'000'000}};

    auto       result = _pTable->Insert()->columns(channelData)->execute();
```

After:
```
    auto result = _pTable->Insert()->columns(&channel)->execute();
```
